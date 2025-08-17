<script setup lang="ts">
import AppLayout from '@/layouts/AppLayout.vue';
import DataTable, { type Column } from '@/components/DataTable.vue';
import { type BreadcrumbItem } from '@/types';
import { Head, router } from '@inertiajs/vue3';
import { Calendar, Clock, CheckCircle, XCircle } from 'lucide-vue-next';
import { ref, computed } from 'vue';

interface Attendance {
    id: number;
    user: {
        id: number;
        name: string;
        email: string;
    };
    check_in: string;
    check_out: string | null;
    date: string;
    status: 'present' | 'late' | 'absent';
    notes: string | null;
}

interface Props {
    attendances?: {
        data: Attendance[];
        links: any[];
        meta: any;
    };
    filters?: {
        search?: string;
        status?: string;
        date?: string;
    };
}

const props = withDefaults(defineProps<Props>(), {
    attendances: () => ({ data: [], links: [], meta: { total: 0, from: 0, to: 0 } }),
    filters: () => ({ search: '', status: '', date: '' })
});

const selectedStatus = ref(props.filters?.status || '');
const selectedDate = ref(props.filters?.date || '');

const breadcrumbs: BreadcrumbItem[] = [
    {
        title: 'Dashboard',
        href: '/dashboard',
    },
    {
        title: 'Data Absensi',
        href: '/attendances',
    },
];

// DataTable columns configuration
const columns: Column[] = [
    {
        key: 'user',
        label: 'Aslab',
        sortable: true,
        width: '25%'
    },
    {
        key: 'date',
        label: 'Tanggal',
        sortable: true,
        width: '15%'
    },
    {
        key: 'check_in',
        label: 'Check In',
        sortable: true,
        width: '15%',
        align: 'center'
    },
    {
        key: 'check_out',
        label: 'Check Out',
        sortable: true,
        width: '15%',
        align: 'center'
    },
    {
        key: 'status',
        label: 'Status',
        sortable: true,
        width: '15%',
        align: 'center'
    },
    {
        key: 'notes',
        label: 'Catatan',
        sortable: false,
        width: '15%'
    }
];

// Computed properties for pagination
const pagination = computed(() => {
    if (!props.attendances?.links || !props.attendances?.meta) return null;

    return {
        current_page: props.attendances.meta.current_page || 1,
        last_page: props.attendances.meta.last_page || 1,
        per_page: props.attendances.meta.per_page || 10,
        total: props.attendances.meta.total || 0,
        from: props.attendances.meta.from || 0,
        to: props.attendances.meta.to || 0,
        links: props.attendances.links
    };
});

// Stats computed
const stats = computed(() => ({
    total: props.attendances?.meta?.total || 0,
    present: props.attendances?.data?.filter(a => a.status === 'present').length || 0,
    late: props.attendances?.data?.filter(a => a.status === 'late').length || 0,
    absent: props.attendances?.data?.filter(a => a.status === 'absent').length || 0
}));

// DataTable event handlers
const handleSearch = (query: string) => {
    router.get('/attendances', {
        search: query || undefined,
        status: selectedStatus.value || undefined,
        date: selectedDate.value || undefined,
    }, {
        preserveState: true,
        replace: true,
    });
};

const handleSort = (column: string | null, direction: 'asc' | 'desc' | null) => {
    router.get('/attendances', {
        search: props.filters?.search || undefined,
        status: selectedStatus.value || undefined,
        date: selectedDate.value || undefined,
        sort: column || undefined,
        direction: direction || undefined,
    }, {
        preserveState: true,
        replace: true,
    });
};

const handleFilter = () => {
    router.get('/attendances', {
        search: props.filters?.search || undefined,
        status: selectedStatus.value || undefined,
        date: selectedDate.value || undefined,
    }, {
        preserveState: true,
        replace: true,
    });
};

const clearFilters = () => {
    selectedStatus.value = '';
    selectedDate.value = '';
    router.get('/attendances', {}, {
        preserveState: true,
        replace: true,
    });
};

const handleRowClick = (row: Attendance) => {
    router.visit(`/attendances/${row.id}`);
};

const formatTime = (time: string | null) => {
    if (!time) return '-';
    return new Date(time).toLocaleTimeString('id-ID', {
        hour: '2-digit',
        minute: '2-digit'
    });
};

const formatDate = (date: string) => {
    return new Date(date).toLocaleDateString('id-ID', {
        weekday: 'long',
        year: 'numeric',
        month: 'long',
        day: 'numeric'
    });
};

const getStatusIcon = (status: string) => {
    switch (status) {
        case 'present': return CheckCircle;
        case 'late': return Clock;
        case 'absent': return XCircle;
        default: return Clock;
    }
};

const getStatusColor = (status: string) => {
    switch (status) {
        case 'present': return 'bg-green-100 text-green-800 dark:bg-green-900/20 dark:text-green-400';
        case 'late': return 'bg-yellow-100 text-yellow-800 dark:bg-yellow-900/20 dark:text-yellow-400';
        case 'absent': return 'bg-red-100 text-red-800 dark:bg-red-900/20 dark:text-red-400';
        default: return 'bg-gray-100 text-gray-800 dark:bg-gray-900/20 dark:text-gray-400';
    }
};

const getStatusText = (status: string) => {
    switch (status) {
        case 'present': return 'Hadir';
        case 'late': return 'Terlambat';
        case 'absent': return 'Tidak Hadir';
        default: return status;
    }
};
</script>

<template>
    <Head title="Data Absensi" />

    <AppLayout :breadcrumbs="breadcrumbs">
        <div class="flex h-full flex-1 flex-col gap-6 rounded-xl p-6">
            <!-- Header -->
            <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between">
                <div>
                    <h1 class="text-2xl font-bold text-foreground">Data Absensi</h1>
                    <p class="mt-1 text-sm text-muted-foreground">Kelola data kehadiran asisten laboratorium</p>
                </div>
            </div>

            <!-- Stats -->
            <div class="grid grid-cols-1 sm:grid-cols-4 gap-4">
                <div class="bg-card rounded-lg border border-border p-4">
                    <div class="flex items-center">
                        <Calendar class="h-8 w-8 text-blue-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Total Absensi</p>
                            <p class="text-2xl font-bold text-foreground">{{ stats.total }}</p>
                        </div>
                    </div>
                </div>
                <div class="bg-card rounded-lg border border-border p-4">
                    <div class="flex items-center">
                        <CheckCircle class="h-8 w-8 text-green-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Hadir</p>
                            <p class="text-2xl font-bold text-foreground">{{ stats.present }}</p>
                        </div>
                    </div>
                </div>
                <div class="bg-card rounded-lg border border-border p-4">
                    <div class="flex items-center">
                        <Clock class="h-8 w-8 text-yellow-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Terlambat</p>
                            <p class="text-2xl font-bold text-foreground">{{ stats.late }}</p>
                        </div>
                    </div>
                </div>
                <div class="bg-card rounded-lg border border-border p-4">
                    <div class="flex items-center">
                        <XCircle class="h-8 w-8 text-red-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Tidak Hadir</p>
                            <p class="text-2xl font-bold text-foreground">{{ stats.absent }}</p>
                        </div>
                    </div>
                </div>
            </div>

            <!-- DataTable -->
            <DataTable
                :columns="columns"
                :data="attendances?.data || []"
                :pagination="pagination || undefined"
                :empty-message="'Tidak ada data absensi ditemukan'"
                :empty-icon="Calendar"
                :show-refresh="true"
                :show-export="true"
                @search="handleSearch"
                @sort="handleSort"
                @refresh="() => router.reload()"
                @row-click="handleRowClick"
            >
                <!-- Filters slot -->
                <template #filters>
                    <div class="flex flex-col sm:flex-row gap-4">
                        <div class="w-full sm:w-48">
                            <select
                                v-model="selectedStatus"
                                @change="handleFilter"
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                            >
                                <option value="">Semua Status</option>
                                <option value="present">Hadir</option>
                                <option value="late">Terlambat</option>
                                <option value="absent">Tidak Hadir</option>
                            </select>
                        </div>
                        <div class="w-full sm:w-48">
                            <input
                                v-model="selectedDate"
                                @change="handleFilter"
                                type="date"
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                            />
                        </div>
                        <button
                            @click="clearFilters"
                            class="px-4 py-2 text-sm text-muted-foreground border border-border rounded-md hover:bg-muted transition-colors"
                        >
                            Reset Filter
                        </button>
                    </div>
                </template>

                <!-- Custom cell templates -->
                <template #cell-user="{ row }">
                    <div>
                        <div class="text-sm font-medium text-foreground">{{ row.user.name }}</div>
                        <div class="text-sm text-muted-foreground">{{ row.user.email }}</div>
                    </div>
                </template>

                <template #cell-date="{ value }">
                    <span class="text-sm text-foreground">{{ formatDate(value) }}</span>
                </template>

                <template #cell-check_in="{ value }">
                    <span class="text-sm text-foreground font-mono">{{ formatTime(value) }}</span>
                </template>

                <template #cell-check_out="{ value }">
                    <span class="text-sm text-foreground font-mono">{{ formatTime(value) }}</span>
                </template>

                <template #cell-status="{ row }">
                    <span :class="[
                        'inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium',
                        getStatusColor(row.status)
                    ]">
                        <component :is="getStatusIcon(row.status)" class="h-3 w-3 mr-1" />
                        {{ getStatusText(row.status) }}
                    </span>
                </template>

                <template #cell-notes="{ value }">
                    <span class="text-sm text-muted-foreground">{{ value || '-' }}</span>
                </template>
            </DataTable>
        </div>
    </AppLayout>
</template>
