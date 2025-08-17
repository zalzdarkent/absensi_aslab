<script setup lang="ts">
import AppLayout from '@/layouts/AppLayout.vue';
import { type BreadcrumbItem } from '@/types';
import { Head, Link, router } from '@inertiajs/vue3';
import { History, Search, Filter, Download, Calendar, Clock, User } from 'lucide-vue-next';
import { ref, watch } from 'vue';

interface User {
    id: number;
    name: string;
    prodi: string;
    semester: number;
}

interface Attendance {
    id: number;
    type: 'check_in' | 'check_out';
    timestamp: string;
    date: string;
    notes: string;
    user: User;
}

interface Props {
    attendances?: {
        data: Attendance[];
        links: any[];
        meta: any;
    };
    prodis?: string[];
    filters?: {
        search?: string;
        prodi?: string;
        date?: string;
    };
}

const props = withDefaults(defineProps<Props>(), {
    attendances: () => ({ data: [], links: [], meta: { total: 0, from: 0, to: 0 } }),
    prodis: () => [],
    filters: () => ({ search: '', prodi: '', date: '' })
});

const search = ref(props.filters?.search || '');
const selectedProdi = ref(props.filters?.prodi || '');
const selectedDate = ref(props.filters?.date || '');

const breadcrumbs: BreadcrumbItem[] = [
    {
        title: 'Dashboard',
        href: '/dashboard',
    },
    {
        title: 'Riwayat Absensi',
        href: '/attendance-history',
    },
];

// Watch for changes and update URL
watch([search, selectedProdi, selectedDate], () => {
    router.get('/attendance-history', {
        search: search.value || undefined,
        prodi: selectedProdi.value || undefined,
        date: selectedDate.value || undefined,
    }, {
        preserveState: true,
        replace: true,
    });
});

const clearFilters = () => {
    search.value = '';
    selectedProdi.value = '';
    selectedDate.value = '';
};

const formatTime = (timestamp: string) => {
    if (!timestamp) return 'N/A';
    try {
        return new Date(timestamp).toLocaleTimeString('id-ID', {
            hour: '2-digit',
            minute: '2-digit',
        });
    } catch {
        return 'N/A';
    }
};

const formatDate = (date: string) => {
    if (!date) return 'N/A';
    try {
        return new Date(date).toLocaleDateString('id-ID', {
            weekday: 'long',
            day: 'numeric',
            month: 'long',
            year: 'numeric',
        });
    } catch {
        return 'N/A';
    }
};

const getTypeColor = (type: string) => {
    if (!type) return 'bg-gray-100 text-gray-800';

    return type === 'check_in'
        ? 'bg-green-100 text-green-800 dark:bg-green-900/20 dark:text-green-400'
        : 'bg-blue-100 text-blue-800 dark:bg-blue-900/20 dark:text-blue-400';
};

const exportData = () => {
    const params = new URLSearchParams({
        search: search.value || '',
        prodi: selectedProdi.value || '',
        date: selectedDate.value || '',
    });
    window.location.href = `/attendance-history/export?${params.toString()}`;
};
</script>

<template>
    <Head title="Riwayat Absensi" />

    <AppLayout :breadcrumbs="breadcrumbs">
        <div class="flex h-full flex-1 flex-col gap-6 rounded-xl p-6 overflow-x-auto">
            <!-- Header -->
            <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between">
                <div>
                    <h1 class="text-2xl font-bold text-foreground flex items-center">
                        <History class="h-7 w-7 mr-3 text-primary" />
                        Riwayat Absensi
                    </h1>
                    <p class="mt-1 text-sm text-muted-foreground">Lihat semua aktivitas absensi aslab</p>
                </div>
                <div class="mt-4 sm:mt-0">
                    <button
                        @click="exportData"
                        class="inline-flex items-center px-4 py-2 bg-green-600 text-white text-sm font-medium rounded-md hover:bg-green-700 transition-colors"
                    >
                        <Download class="h-4 w-4 mr-2" />
                        Export Data
                    </button>
                </div>
            </div>

            <!-- Filters -->
            <div class="bg-card rounded-lg border border-border p-4">
                <div class="grid grid-cols-1 md:grid-cols-4 gap-4">
                    <div class="md:col-span-2">
                        <div class="relative">
                            <Search class="absolute left-3 top-1/2 transform -translate-y-1/2 h-4 w-4 text-muted-foreground" />
                            <input
                                v-model="search"
                                type="text"
                                placeholder="Cari nama aslab..."
                                class="w-full pl-10 pr-4 py-2 border border-border rounded-md bg-background text-foreground placeholder:text-muted-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                            />
                        </div>
                    </div>
                    <div>
                        <select
                            v-model="selectedProdi"
                            class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                        >
                            <option value="">Semua Prodi</option>
                            <option v-for="prodi in (prodis || [])" :key="prodi" :value="prodi">
                                {{ prodi }}
                            </option>
                        </select>
                    </div>
                    <div class="flex space-x-2">
                        <input
                            v-model="selectedDate"
                            type="date"
                            class="flex-1 px-3 py-2 border border-border rounded-md bg-background text-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                        />
                        <button
                            @click="clearFilters"
                            class="px-4 py-2 text-sm text-muted-foreground border border-border rounded-md hover:bg-muted transition-colors"
                        >
                            <Filter class="h-4 w-4" />
                        </button>
                    </div>
                </div>
            </div>

            <!-- Stats -->
            <div class="grid grid-cols-1 sm:grid-cols-3 gap-4">
                <div class="bg-card rounded-lg border border-border p-4">
                    <div class="flex items-center">
                        <Calendar class="h-8 w-8 text-blue-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Total Records</p>
                            <p class="text-2xl font-bold text-foreground">{{ attendances?.meta?.total || 0 }}</p>
                        </div>
                    </div>
                </div>
                <div class="bg-card rounded-lg border border-border p-4">
                    <div class="flex items-center">
                        <Clock class="h-8 w-8 text-green-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Check-ins</p>
                            <p class="text-2xl font-bold text-foreground">
                                {{ attendances?.data?.filter(a => a.type === 'check_in').length || 0 }}
                            </p>
                        </div>
                    </div>
                </div>
                <div class="bg-card rounded-lg border border-border p-4">
                    <div class="flex items-center">
                        <Clock class="h-8 w-8 text-blue-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Check-outs</p>
                            <p class="text-2xl font-bold text-foreground">
                                {{ attendances?.data?.filter(a => a.type === 'check_out').length || 0 }}
                            </p>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Table -->
            <div class="bg-card rounded-lg border border-border overflow-hidden">
                <div class="overflow-x-auto">
                    <table class="w-full">
                        <thead class="bg-muted/50">
                            <tr>
                                <th class="px-6 py-3 text-left text-xs font-medium text-muted-foreground uppercase tracking-wider">
                                    Tanggal & Waktu
                                </th>
                                <th class="px-6 py-3 text-left text-xs font-medium text-muted-foreground uppercase tracking-wider">
                                    Aslab
                                </th>
                                <th class="px-6 py-3 text-left text-xs font-medium text-muted-foreground uppercase tracking-wider">
                                    Tipe
                                </th>
                                <th class="px-6 py-3 text-left text-xs font-medium text-muted-foreground uppercase tracking-wider">
                                    Catatan
                                </th>
                                <th class="px-6 py-3 text-left text-xs font-medium text-muted-foreground uppercase tracking-wider">
                                    Aksi
                                </th>
                            </tr>
                        </thead>
                        <tbody class="divide-y divide-border">
                            <tr v-if="!attendances?.data || attendances.data.length === 0">
                                <td colspan="5" class="px-6 py-8 text-center text-muted-foreground">
                                    <History class="mx-auto h-12 w-12 text-muted-foreground/50" />
                                    <p class="mt-2">Tidak ada data absensi ditemukan</p>
                                </td>
                            </tr>
                            <tr v-else v-for="attendance in (attendances?.data || [])" :key="attendance?.id || Math.random()" class="hover:bg-muted/30 transition-colors">
                                <td class="px-6 py-4">
                                    <div>
                                        <div class="text-sm font-medium text-foreground">
                                            {{ formatDate(attendance?.date) }}
                                        </div>
                                        <div class="text-sm text-muted-foreground">
                                            {{ formatTime(attendance?.timestamp) }}
                                        </div>
                                    </div>
                                </td>
                                <td class="px-6 py-4">
                                    <div class="flex items-center">
                                        <User class="h-8 w-8 text-muted-foreground bg-muted rounded-full p-1 mr-3" />
                                        <div>
                                            <div class="text-sm font-medium text-foreground">{{ attendance.user?.name || 'N/A' }}</div>
                                            <div class="text-sm text-muted-foreground">
                                                {{ attendance.user?.prodi || 'N/A' }} - Semester {{ attendance.user?.semester || 'N/A' }}
                                            </div>
                                        </div>
                                    </div>
                                </td>
                                <td class="px-6 py-4">
                                    <span :class="['inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium', getTypeColor(attendance?.type)]">
                                        {{ attendance?.type === 'check_in' ? 'Check-in' : 'Check-out' }}
                                    </span>
                                </td>
                                <td class="px-6 py-4 text-sm text-foreground">
                                    {{ attendance?.notes || '-' }}
                                </td>
                                <td class="px-6 py-4">
                                    <Link
                                        v-if="attendance.user?.id"
                                        :href="`/aslabs/${attendance.user.id}`"
                                        class="text-blue-600 hover:text-blue-800 dark:text-blue-400 dark:hover:text-blue-300 text-sm font-medium transition-colors"
                                    >
                                        Lihat Detail
                                    </Link>
                                    <span v-else class="text-muted-foreground text-sm">
                                        N/A
                                    </span>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </div>

                <!-- Pagination -->
                <div v-if="attendances?.links && attendances.links.length > 3" class="bg-muted/30 px-6 py-3 border-t border-border">
                    <div class="flex items-center justify-between">
                        <div class="text-sm text-muted-foreground">
                            Menampilkan {{ attendances?.meta?.from || 0 }} - {{ attendances?.meta?.to || 0 }} dari {{ attendances?.meta?.total || 0 }} data
                        </div>
                        <div class="flex space-x-1">
                            <Link
                                v-for="link in (attendances?.links || [])"
                                :key="link.label"
                                :href="link.url"
                                :class="[
                                    'px-3 py-2 text-sm rounded-md transition-colors',
                                    link.active
                                        ? 'bg-primary text-primary-foreground'
                                        : link.url
                                        ? 'text-muted-foreground hover:bg-muted hover:text-foreground'
                                        : 'text-muted-foreground/50 cursor-not-allowed'
                                ]"
                                :preserve-scroll="true"
                            >
                                <span v-if="link.label.includes('Previous')">&larr; Sebelumnya</span>
                                <span v-else-if="link.label.includes('Next')">Selanjutnya &rarr;</span>
                                <span v-else>{{ link.label }}</span>
                            </Link>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </AppLayout>
</template>
