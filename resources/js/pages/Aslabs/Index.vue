<script setup lang="ts">
import AppLayout from '@/layouts/AppLayout.vue';
import DataTable, { type Column } from '@/components/DataTable.vue';
import { type BreadcrumbItem } from '@/types';
import { Head, Link, router } from '@inertiajs/vue3';
import { Users, Plus, Eye, Edit, ToggleLeft, ToggleRight } from 'lucide-vue-next';
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

interface Props {
    aslabs?: User[];
    prodis?: string[];
    filters?: {
        search?: string;
        prodi?: string;
    };
}

const props = withDefaults(defineProps<Props>(), {
    aslabs: () => [],
    prodis: () => [],
    filters: () => ({ search: '', prodi: '' })
});

const selectedProdi = ref(props.filters?.prodi || '');

const breadcrumbs: BreadcrumbItem[] = [
    {
        title: 'Dashboard',
        href: '/dashboard',
    },
    {
        title: 'Data Aslab',
        href: '/aslabs',
    },
];

// DataTable columns configuration
const columns: Column[] = [
    {
        key: 'name',
        label: 'Aslab',
        sortable: true,
        width: '25%',
        searchable: true
    },
    {
        key: 'rfid_code',
        label: 'Kode RFID',
        sortable: true,
        width: '15%',
        searchable: true
    },
    {
        key: 'prodi',
        label: 'Program Studi',
        sortable: true,
        width: '20%',
        searchable: true
    },
    {
        key: 'semester',
        label: 'Semester',
        sortable: true,
        width: '10%',
        align: 'center',
        searchable: true
    },
    {
        key: 'is_active',
        label: 'Status',
        sortable: true,
        width: '15%',
        align: 'center',
        searchable: false
    },
    {
        key: 'actions',
        label: 'Aksi',
        sortable: false,
        width: '15%',
        align: 'center',
        searchable: false
    }
];

// Computed properties (pagination removed since DataTable handles it client-side)
const stats = computed(() => ({
    total: props.aslabs?.length || 0,
    active: props.aslabs?.filter((a: User) => a.is_active).length || 0,
    inactive: props.aslabs?.filter((a: User) => !a.is_active).length || 0
}));

// DataTable event handlers (search and sort now handled by DataTable internally)
const handleFilter = () => {
    router.get('/aslabs', {
        search: props.filters?.search || undefined,
        prodi: selectedProdi.value || undefined,
    }, {
        preserveState: true,
        replace: true,
    });
};

const clearFilters = () => {
    selectedProdi.value = '';
    router.get('/aslabs', {}, {
        preserveState: true,
        replace: true,
    });
};

const toggleStatus = (aslab: User) => {
    router.patch(`/aslabs/${aslab.id}/toggle-status`, {}, {
        preserveScroll: true,
    });
};

const handleRowClick = (row: User) => {
    router.visit(`/aslabs/${row.id}`);
};
</script>

<template>
    <Head title="Data Aslab" />

    <AppLayout :breadcrumbs="breadcrumbs">
        <div class="flex h-full flex-1 flex-col gap-6 rounded-xl p-6">
            <!-- Header -->
            <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between">
                <div>
                    <h1 class="text-2xl font-bold text-foreground">Data Aslab</h1>
                    <p class="mt-1 text-sm text-muted-foreground">Kelola data asisten laboratorium</p>
                </div>
                <div class="mt-4 sm:mt-0">
                    <Link
                        href="/aslabs/create"
                        class="inline-flex items-center px-4 py-2 bg-primary text-primary-foreground text-sm font-medium rounded-md hover:bg-primary/90 transition-colors"
                    >
                        <Plus class="h-4 w-4 mr-2" />
                        Tambah Aslab
                    </Link>
                </div>
            </div>

            <!-- Stats -->
            <div class="grid grid-cols-1 sm:grid-cols-3 gap-4">
                <div class="bg-card rounded-lg border border-border p-4">
                    <div class="flex items-center">
                        <Users class="h-8 w-8 text-green-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Total Aslab</p>
                            <p class="text-2xl font-bold text-foreground">{{ stats.total }}</p>
                        </div>
                    </div>
                </div>
                <div class="bg-card rounded-lg border border-border p-4">
                    <div class="flex items-center">
                        <ToggleRight class="h-8 w-8 text-blue-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Aktif</p>
                            <p class="text-2xl font-bold text-foreground">{{ stats.active }}</p>
                        </div>
                    </div>
                </div>
                <div class="bg-card rounded-lg border border-border p-4">
                    <div class="flex items-center">
                        <ToggleLeft class="h-8 w-8 text-gray-500" />
                        <div class="ml-4">
                            <p class="text-sm text-muted-foreground">Non-Aktif</p>
                            <p class="text-2xl font-bold text-foreground">{{ stats.inactive }}</p>
                        </div>
                    </div>
                </div>
            </div>

            <!-- DataTable -->
            <DataTable
                :columns="columns"
                :data="aslabs || []"
                :empty-message="'Tidak ada data aslab ditemukan'"
                :empty-icon="Users"
                :show-refresh="true"
                @refresh="() => router.reload()"
                @row-click="handleRowClick"
            >
                <!-- Filters slot -->
                <template #filters>
                    <div class="flex flex-col sm:flex-row gap-4">
                        <div class="w-full sm:w-48">
                            <select
                                v-model="selectedProdi"
                                @change="handleFilter"
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                            >
                                <option value="">Semua Prodi</option>
                                <option v-for="prodi in (prodis || [])" :key="prodi" :value="prodi">
                                    {{ prodi }}
                                </option>
                            </select>
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
                <template #cell-name="{ row }">
                    <div>
                        <div class="text-sm font-medium text-foreground">{{ row.name }}</div>
                        <div class="text-sm text-muted-foreground">{{ row.email }}</div>
                    </div>
                </template>

                <template #cell-rfid_code="{ value }">
                    <span class="inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium bg-primary/10 text-primary">
                        {{ value }}
                    </span>
                </template>

                <template #cell-semester="{ value }">
                    <span class="text-sm text-foreground">{{ value }}</span>
                </template>

                <template #cell-is_active="{ row }">
                    <button
                        @click.stop="toggleStatus(row)"
                        :class="[
                            'inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium transition-colors',
                            row.is_active
                                ? 'bg-green-100 text-green-800 dark:bg-green-900/20 dark:text-green-400'
                                : 'bg-gray-100 text-gray-800 dark:bg-gray-900/20 dark:text-gray-400'
                        ]"
                    >
                        <component :is="row.is_active ? ToggleRight : ToggleLeft" class="h-3 w-3 mr-1" />
                        {{ row.is_active ? 'Aktif' : 'Non-Aktif' }}
                    </button>
                </template>

                <template #cell-actions="{ row }">
                    <div class="flex items-center justify-center space-x-2">
                        <Link
                            :href="`/aslabs/${row.id}`"
                            @click.stop
                            class="p-1 text-blue-600 hover:text-blue-800 dark:text-blue-400 dark:hover:text-blue-300 transition-colors"
                            title="Lihat Detail"
                        >
                            <Eye class="h-4 w-4" />
                        </Link>
                        <Link
                            :href="`/aslabs/${row.id}/edit`"
                            @click.stop
                            class="p-1 text-green-600 hover:text-green-800 dark:text-green-400 dark:hover:text-green-300 transition-colors"
                            title="Edit"
                        >
                            <Edit class="h-4 w-4" />
                        </Link>
                    </div>
                </template>
            </DataTable>
        </div>
    </AppLayout>
</template>
