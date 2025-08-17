<script setup lang="ts">
import DataTable, { type Column } from '@/components/DataTable.vue';
import { Users, Eye, Edit, Trash2, UserPlus } from 'lucide-vue-next';
import { ref, computed } from 'vue';

// Contoh data untuk demo
const sampleData = ref([
    {
        id: 1,
        name: 'Ahmad Fauzi',
        email: 'ahmad.fauzi@example.com',
        rfid_code: 'RF001234',
        prodi: 'Teknik Informatika',
        semester: 6,
        is_active: true,
        created_at: '2024-01-15T08:30:00Z'
    },
    {
        id: 2,
        name: 'Siti Nurhaliza',
        email: 'siti.nurhaliza@example.com',
        rfid_code: 'RF001235',
        prodi: 'Sistem Informasi',
        semester: 4,
        is_active: true,
        created_at: '2024-01-16T09:15:00Z'
    },
    {
        id: 3,
        name: 'Budi Santoso',
        email: 'budi.santoso@example.com',
        rfid_code: 'RF001236',
        prodi: 'Teknik Komputer',
        semester: 8,
        is_active: false,
        created_at: '2024-01-17T10:45:00Z'
    }
]);

const isLoading = ref(false);

// Column configuration dengan advanced features
const columns: Column[] = [
    {
        key: 'profile',
        label: 'Profile',
        sortable: true,
        width: '25%',
        render: (value, row) => `${row.name} (${row.email})`
    },
    {
        key: 'rfid_code',
        label: 'RFID Code',
        sortable: true,
        width: '12%',
        align: 'center'
    },
    {
        key: 'academic',
        label: 'Academic Info',
        sortable: false,
        width: '20%'
    },
    {
        key: 'status',
        label: 'Status',
        sortable: true,
        width: '12%',
        align: 'center'
    },
    {
        key: 'created_at',
        label: 'Bergabung',
        sortable: true,
        width: '15%',
        align: 'center'
    },
    {
        key: 'actions',
        label: 'Aksi',
        sortable: false,
        width: '16%',
        align: 'center'
    }
];

// Mock pagination
const pagination = computed(() => ({
    current_page: 1,
    last_page: 1,
    per_page: 10,
    total: sampleData.value.length,
    from: 1,
    to: sampleData.value.length,
    links: [
        { url: null, label: '&laquo; Previous', active: false },
        { url: '#', label: '1', active: true },
        { url: null, label: 'Next &raquo;', active: false }
    ]
}));

// Event handlers
const handleSearch = (query: string) => {
    console.log('Search:', query);
    // Implement search logic
};

const handleSort = (column: string | null, direction: 'asc' | 'desc' | null) => {
    console.log('Sort:', column, direction);
    // Implement sort logic
};

const handleRefresh = () => {
    isLoading.value = true;
    console.log('Refreshing data...');

    // Simulate API call
    setTimeout(() => {
        isLoading.value = false;
        console.log('Data refreshed!');
    }, 2000);
};

const handleExport = (format: 'csv' | 'excel') => {
    console.log('Export as:', format);
    // Implement export logic
};

const handleRowClick = (row: any) => {
    console.log('Row clicked:', row);
    // Navigate to detail page
};

// Action handlers
const viewUser = (user: any) => {
    console.log('View user:', user.name);
};

const editUser = (user: any) => {
    console.log('Edit user:', user.name);
};

const deleteUser = (user: any) => {
    if (confirm(`Hapus user ${user.name}?`)) {
        console.log('Delete user:', user.name);
        // Implement delete logic
    }
};

const toggleStatus = (user: any) => {
    user.is_active = !user.is_active;
    console.log('Toggle status:', user.name, user.is_active);
};

// Utility functions
const formatDate = (dateString: string) => {
    return new Date(dateString).toLocaleDateString('id-ID', {
        year: 'numeric',
        month: 'short',
        day: 'numeric'
    });
};

const getStatusColor = (isActive: boolean) => {
    return isActive
        ? 'bg-green-100 text-green-800 dark:bg-green-900/20 dark:text-green-400'
        : 'bg-gray-100 text-gray-800 dark:bg-gray-900/20 dark:text-gray-400';
};
</script>

<template>
    <div class="p-8 space-y-6">
        <!-- Header -->
        <div class="flex justify-between items-center">
            <div>
                <h1 class="text-3xl font-bold text-gray-900 dark:text-white">DataTable Demo</h1>
                <p class="text-gray-600 dark:text-gray-400">Contoh implementasi DataTable component yang reusable</p>
            </div>
            <button class="flex items-center gap-2 px-4 py-2 bg-blue-600 text-white rounded-lg hover:bg-blue-700">
                <UserPlus class="h-4 w-4" />
                Tambah User
            </button>
        </div>

        <!-- Features Overview -->
        <div class="bg-blue-50 dark:bg-blue-900/20 border border-blue-200 dark:border-blue-800 rounded-lg p-4">
            <h3 class="font-semibold text-blue-900 dark:text-blue-100 mb-2">Features Demo:</h3>
            <ul class="text-sm text-blue-800 dark:text-blue-200 space-y-1">
                <li>✅ Sortable columns (klik header untuk sort)</li>
                <li>✅ Global search dengan debouncing</li>
                <li>✅ Custom cell rendering dengan slots</li>
                <li>✅ Action buttons dengan event handling</li>
                <li>✅ Loading states dan refresh functionality</li>
                <li>✅ Responsive design dan pagination</li>
                <li>✅ Row click navigation</li>
                <li>✅ Custom filters dan empty states</li>
            </ul>
        </div>

        <!-- DataTable Implementation -->
        <DataTable
            :columns="columns"
            :data="sampleData"
            :pagination="pagination"
            :loading="isLoading"
            :empty-message="'Belum ada data user'"
            :empty-icon="Users"
            :show-refresh="true"
            :show-export="true"
            @search="handleSearch"
            @sort="handleSort"
            @refresh="handleRefresh"
            @export="handleExport"
            @row-click="handleRowClick"
        >
            <!-- Custom filters -->
            <template #filters>
                <div class="flex flex-wrap gap-4">
                    <select class="px-3 py-2 border border-gray-300 rounded-md bg-white text-gray-900 focus:ring-2 focus:ring-blue-500 focus:border-blue-500">
                        <option value="">Semua Prodi</option>
                        <option value="ti">Teknik Informatika</option>
                        <option value="si">Sistem Informasi</option>
                        <option value="tk">Teknik Komputer</option>
                    </select>

                    <select class="px-3 py-2 border border-gray-300 rounded-md bg-white text-gray-900 focus:ring-2 focus:ring-blue-500 focus:border-blue-500">
                        <option value="">Semua Status</option>
                        <option value="active">Aktif</option>
                        <option value="inactive">Non-Aktif</option>
                    </select>

                    <input
                        type="number"
                        placeholder="Semester..."
                        min="1"
                        max="8"
                        class="px-3 py-2 border border-gray-300 rounded-md bg-white text-gray-900 focus:ring-2 focus:ring-blue-500 focus:border-blue-500"
                    />

                    <button class="px-4 py-2 text-sm text-gray-600 border border-gray-300 rounded-md hover:bg-gray-50">
                        Reset Filter
                    </button>
                </div>
            </template>

            <!-- Custom cell: Profile dengan avatar dan info -->
            <template #cell-profile="{ row }">
                <div class="flex items-center space-x-3">
                    <div class="flex-shrink-0 h-10 w-10">
                        <div class="h-10 w-10 rounded-full bg-gradient-to-br from-blue-400 to-purple-500 flex items-center justify-center text-white font-semibold">
                            {{ row.name.charAt(0).toUpperCase() }}
                        </div>
                    </div>
                    <div>
                        <div class="text-sm font-medium text-gray-900 dark:text-white">{{ row.name }}</div>
                        <div class="text-sm text-gray-500 dark:text-gray-400">{{ row.email }}</div>
                    </div>
                </div>
            </template>

            <!-- Custom cell: RFID Code dengan styling -->
            <template #cell-rfid_code="{ value }">
                <span class="inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-mono font-medium bg-purple-100 text-purple-800 dark:bg-purple-900/20 dark:text-purple-400">
                    {{ value }}
                </span>
            </template>

            <!-- Custom cell: Academic info -->
            <template #cell-academic="{ row }">
                <div class="text-sm">
                    <div class="font-medium text-gray-900 dark:text-white">{{ row.prodi }}</div>
                    <div class="text-gray-500 dark:text-gray-400">Semester {{ row.semester }}</div>
                </div>
            </template>

            <!-- Custom cell: Status dengan toggle -->
            <template #cell-status="{ row }">
                <button
                    @click.stop="toggleStatus(row)"
                    :class="[
                        'inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium transition-all duration-200 hover:scale-105',
                        getStatusColor(row.is_active)
                    ]"
                >
                    <div
                        :class="[
                            'w-2 h-2 rounded-full mr-1.5',
                            row.is_active ? 'bg-green-500' : 'bg-gray-400'
                        ]"
                    ></div>
                    {{ row.is_active ? 'Aktif' : 'Non-Aktif' }}
                </button>
            </template>

            <!-- Custom cell: Date formatting -->
            <template #cell-created_at="{ value }">
                <span class="text-sm text-gray-900 dark:text-white">{{ formatDate(value) }}</span>
            </template>

            <!-- Custom cell: Actions dengan multiple buttons -->
            <template #cell-actions="{ row }">
                <div class="flex items-center justify-center space-x-1">
                    <button
                        @click.stop="viewUser(row)"
                        class="p-1.5 text-blue-600 hover:text-blue-800 hover:bg-blue-100 dark:hover:bg-blue-900/20 rounded transition-colors"
                        title="Lihat Detail"
                    >
                        <Eye class="h-4 w-4" />
                    </button>

                    <button
                        @click.stop="editUser(row)"
                        class="p-1.5 text-green-600 hover:text-green-800 hover:bg-green-100 dark:hover:bg-green-900/20 rounded transition-colors"
                        title="Edit"
                    >
                        <Edit class="h-4 w-4" />
                    </button>

                    <button
                        @click.stop="deleteUser(row)"
                        class="p-1.5 text-red-600 hover:text-red-800 hover:bg-red-100 dark:hover:bg-red-900/20 rounded transition-colors"
                        title="Hapus"
                    >
                        <Trash2 class="h-4 w-4" />
                    </button>
                </div>
            </template>
        </DataTable>

        <!-- Implementation Notes -->
        <div class="bg-gray-50 dark:bg-gray-800 rounded-lg p-6">
            <h3 class="font-semibold text-gray-900 dark:text-white mb-3">Implementation Notes:</h3>
            <div class="grid md:grid-cols-2 gap-4 text-sm text-gray-600 dark:text-gray-400">
                <div>
                    <h4 class="font-medium text-gray-900 dark:text-white mb-2">Features Used:</h4>
                    <ul class="space-y-1">
                        <li>• Custom column configuration</li>
                        <li>• Slot-based cell rendering</li>
                        <li>• Event handling untuk actions</li>
                        <li>• Loading states dan refresh</li>
                        <li>• Custom filters dengan multiple inputs</li>
                    </ul>
                </div>
                <div>
                    <h4 class="font-medium text-gray-900 dark:text-white mb-2">Code Highlights:</h4>
                    <ul class="space-y-1">
                        <li>• @click.stop untuk prevent row click</li>
                        <li>• Responsive design dengan Tailwind</li>
                        <li>• Dark mode compatibility</li>
                        <li>• TypeScript untuk type safety</li>
                        <li>• Reusable dan modular architecture</li>
                    </ul>
                </div>
            </div>
        </div>
    </div>
</template>
