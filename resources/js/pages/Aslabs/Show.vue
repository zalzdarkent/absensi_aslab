<script setup lang="ts">
import AppLayout from '@/layouts/AppLayout.vue';
import { type BreadcrumbItem } from '@/types';
import { Head, Link, router } from '@inertiajs/vue3';
import { Users, Clock, Calendar, ArrowLeft, Filter, Download, TrendingUp } from 'lucide-vue-next';
import { ref, computed } from 'vue';

interface User {
    id: number;
    name: string;
    email: string;
    rfid_code: string;
    prodi: string;
    semester: number;
    role: string;
    is_active: boolean;
    created_at: string;
}

interface Attendance {
    id: number;
    type: 'check_in' | 'check_out';
    timestamp: string;
    date: string;
    notes: string;
}

interface Props {
    aslab: User;
    stats: {
        total_days: number;
        this_month: number;
    };
    recent_attendances: Attendance[];
}

const props = defineProps<Props>();

const selectedMonth = ref(new Date().getMonth() + 1);
const selectedYear = ref(new Date().getFullYear());

const breadcrumbs: BreadcrumbItem[] = [
    {
        title: 'Dashboard',
        href: '/dashboard',
    },
    {
        title: 'Data Aslab',
        href: '/aslabs',
    },
    {
        title: props.aslab.name,
        href: `/aslabs/${props.aslab.id}`,
    },
];

const months = [
    { value: 1, label: 'Januari' },
    { value: 2, label: 'Februari' },
    { value: 3, label: 'Maret' },
    { value: 4, label: 'April' },
    { value: 5, label: 'Mei' },
    { value: 6, label: 'Juni' },
    { value: 7, label: 'Juli' },
    { value: 8, label: 'Agustus' },
    { value: 9, label: 'September' },
    { value: 10, label: 'Oktober' },
    { value: 11, label: 'November' },
    { value: 12, label: 'Desember' },
];

const years = computed(() => {
    const currentYear = new Date().getFullYear();
    const years = [];
    for (let i = currentYear - 5; i <= currentYear + 1; i++) {
        years.push(i);
    }
    return years;
});

const groupedAttendances = computed(() => {
    const grouped: { [date: string]: Attendance[] } = {};

    props.recent_attendances.forEach(attendance => {
        const date = attendance.date;
        if (!grouped[date]) {
            grouped[date] = [];
        }
        grouped[date].push(attendance);
    });

    return grouped;
});

const getStatusBadge = (aslab: User) => {
    if (aslab.is_active) {
        return 'bg-green-100 text-green-800 dark:bg-green-900/20 dark:text-green-400';
    }
    return 'bg-gray-100 text-gray-800 dark:bg-gray-900/20 dark:text-gray-400';
};

const formatTime = (timestamp: string) => {
    return new Date(timestamp).toLocaleTimeString('id-ID', {
        hour: '2-digit',
        minute: '2-digit',
    });
};

const formatDate = (date: string) => {
    return new Date(date).toLocaleDateString('id-ID', {
        weekday: 'long',
        day: 'numeric',
        month: 'long',
        year: 'numeric',
    });
};

const loadAttendanceData = () => {
    router.get(`/aslabs/${props.aslab.id}`, {
        month: selectedMonth.value,
        year: selectedYear.value,
    }, {
        preserveState: true,
        replace: true,
    });
};

const exportData = () => {
    window.location.href = `/aslabs/${props.aslab.id}/export?month=${selectedMonth.value}&year=${selectedYear.value}`;
};
</script>

<template>
    <Head :title="`Detail ${aslab.name}`" />

    <AppLayout :breadcrumbs="breadcrumbs">
        <div class="flex h-full flex-1 flex-col gap-6 rounded-xl p-6 overflow-x-auto">
            <!-- Header -->
            <div class="flex flex-col sm:flex-row sm:items-start sm:justify-between">
                <div class="flex items-start space-x-4">
                    <Link
                        href="/aslabs"
                        class="p-2 bg-muted rounded-lg hover:bg-muted/80 transition-colors"
                    >
                        <ArrowLeft class="h-5 w-5 text-muted-foreground" />
                    </Link>
                    <div>
                        <h1 class="text-2xl font-bold text-foreground">{{ aslab.name }}</h1>
                        <p class="mt-1 text-sm text-muted-foreground">Detail dan riwayat absensi aslab</p>
                    </div>
                </div>
                <div class="mt-4 sm:mt-0 flex space-x-2">
                    <button
                        @click="exportData"
                        class="inline-flex items-center px-4 py-2 bg-green-600 text-white text-sm font-medium rounded-md hover:bg-green-700 transition-colors"
                    >
                        <Download class="h-4 w-4 mr-2" />
                        Export Data
                    </button>
                    <Link
                        :href="`/aslabs/${aslab.id}/edit`"
                        class="inline-flex items-center px-4 py-2 bg-primary text-primary-foreground text-sm font-medium rounded-md hover:bg-primary/90 transition-colors"
                    >
                        Edit Data
                    </Link>
                </div>
            </div>

            <!-- Profile Card -->
            <div class="bg-card rounded-lg border border-border p-6">
                <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between">
                    <div class="flex items-center space-x-4">
                        <div class="w-16 h-16 bg-primary/10 rounded-full flex items-center justify-center">
                            <Users class="h-8 w-8 text-primary" />
                        </div>
                        <div>
                            <h2 class="text-xl font-semibold text-foreground">{{ aslab.name }}</h2>
                            <p class="text-muted-foreground">{{ aslab.email }}</p>
                            <div class="flex items-center space-x-4 mt-2">
                                <span class="text-sm text-muted-foreground">
                                    <strong>RFID:</strong> {{ aslab.rfid_code }}
                                </span>
                                <span class="text-sm text-muted-foreground">
                                    <strong>Prodi:</strong> {{ aslab.prodi }}
                                </span>
                                <span class="text-sm text-muted-foreground">
                                    <strong>Semester:</strong> {{ aslab.semester }}
                                </span>
                            </div>
                        </div>
                    </div>
                    <div class="mt-4 sm:mt-0">
                        <span :class="['inline-flex items-center px-3 py-1 rounded-full text-sm font-medium', getStatusBadge(aslab)]">
                            {{ aslab.is_active ? 'Aktif' : 'Non-Aktif' }}
                        </span>
                    </div>
                </div>
            </div>

            <!-- Statistics -->
            <div class="grid grid-cols-1 sm:grid-cols-3 gap-4">
                <div class="bg-card rounded-lg border border-border p-6">
                    <div class="flex items-center">
                        <Calendar class="h-8 w-8 text-blue-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Total Hari Absen</p>
                            <p class="text-2xl font-bold text-foreground">{{ stats.total_days }}</p>
                        </div>
                    </div>
                </div>
                <div class="bg-card rounded-lg border border-border p-6">
                    <div class="flex items-center">
                        <TrendingUp class="h-8 w-8 text-green-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Bulan Ini</p>
                            <p class="text-2xl font-bold text-foreground">{{ stats.this_month }}</p>
                        </div>
                    </div>
                </div>
                <div class="bg-card rounded-lg border border-border p-6">
                    <div class="flex items-center">
                        <Clock class="h-8 w-8 text-indigo-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Rata-rata/Bulan</p>
                            <p class="text-2xl font-bold text-foreground">
                                {{ Math.round(stats.total_days / Math.max(1, new Date().getMonth() + 1)) }}
                            </p>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Filter -->
            <div class="bg-card rounded-lg border border-border p-4">
                <div class="flex items-center space-x-4">
                    <Filter class="h-5 w-5 text-muted-foreground" />
                    <span class="text-sm font-medium text-foreground">Filter Periode:</span>
                    <select
                        v-model="selectedMonth"
                        class="px-3 py-2 border border-border rounded-md bg-background text-foreground focus:outline-none focus:ring-2 focus:ring-primary"
                    >
                        <option v-for="month in months" :key="month.value" :value="month.value">
                            {{ month.label }}
                        </option>
                    </select>
                    <select
                        v-model="selectedYear"
                        class="px-3 py-2 border border-border rounded-md bg-background text-foreground focus:outline-none focus:ring-2 focus:ring-primary"
                    >
                        <option v-for="year in years" :key="year" :value="year">
                            {{ year }}
                        </option>
                    </select>
                    <button
                        @click="loadAttendanceData"
                        class="px-4 py-2 bg-primary text-primary-foreground text-sm font-medium rounded-md hover:bg-primary/90 transition-colors"
                    >
                        Tampilkan
                    </button>
                </div>
            </div>

            <!-- Attendance History -->
            <div class="bg-card rounded-lg border border-border">
                <div class="p-6 border-b border-border">
                    <h3 class="text-lg font-medium text-foreground">Riwayat Absensi</h3>
                    <p class="text-sm text-muted-foreground">
                        {{ months.find(m => m.value === selectedMonth)?.label }} {{ selectedYear }}
                    </p>
                </div>
                <div class="p-6">
                    <div v-if="Object.keys(groupedAttendances).length === 0" class="text-center py-8">
                        <Calendar class="mx-auto h-12 w-12 text-muted-foreground/50" />
                        <p class="mt-2 text-muted-foreground">Tidak ada data absensi untuk periode ini</p>
                    </div>
                    <div v-else class="space-y-4">
                        <div
                            v-for="(attendances, date) in groupedAttendances"
                            :key="date"
                            class="border border-border rounded-lg p-4"
                        >
                            <div class="flex items-center justify-between mb-3">
                                <h4 class="font-medium text-foreground">{{ formatDate(date) }}</h4>
                                <span class="text-sm text-muted-foreground">
                                    {{ attendances.length }} aktivitas
                                </span>
                            </div>
                            <div class="grid grid-cols-1 sm:grid-cols-2 gap-4">
                                <div
                                    v-for="attendance in attendances"
                                    :key="attendance.id"
                                    class="flex items-center justify-between p-3 bg-muted/30 rounded-lg"
                                >
                                    <div class="flex items-center space-x-3">
                                        <div :class="[
                                            'w-3 h-3 rounded-full',
                                            attendance.type === 'check_in' ? 'bg-green-500' : 'bg-blue-500'
                                        ]"></div>
                                        <div>
                                            <p class="text-sm font-medium text-foreground">
                                                {{ attendance.type === 'check_in' ? 'Check-in' : 'Check-out' }}
                                            </p>
                                            <p class="text-xs text-muted-foreground">{{ attendance.notes }}</p>
                                        </div>
                                    </div>
                                    <span class="text-sm font-mono text-foreground">
                                        {{ formatTime(attendance.timestamp) }}
                                    </span>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </AppLayout>
</template>
