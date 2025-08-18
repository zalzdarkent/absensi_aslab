<script setup lang="ts">
import { ref, computed, watch } from 'vue';
import {
    ChevronUp,
    ChevronDown,
    ChevronsUpDown,
    Search,
    Filter,
    Download,
    RefreshCw,
    ChevronLeft,
    ChevronRight,
    ChevronsLeft,
    ChevronsRight
} from 'lucide-vue-next';

export interface Column {
    key: string;
    label: string;
    sortable?: boolean;
    searchable?: boolean;
    width?: string;
    align?: 'left' | 'center' | 'right';
    render?: (value: any, row: any) => any;
}

export interface ServerPagination {
    current_page: number;
    last_page: number;
    per_page: number;
    total: number;
    from: number;
    to: number;
    links: any[];
}

export interface DataTableProps {
    columns: Column[];
    data: any[];
    loading?: boolean;
    searchable?: boolean;
    sortable?: boolean;
    filters?: Record<string, any>;
    emptyMessage?: string;
    emptyIcon?: any;
    showRefresh?: boolean;
    showExport?: boolean;
    exportFilename?: string;
    itemsPerPageOptions?: number[];
    defaultItemsPerPage?: number;
    pagination?: ServerPagination;
}

const props = withDefaults(defineProps<DataTableProps>(), {
    loading: false,
    searchable: true,
    sortable: true,
    emptyMessage: 'Tidak ada data ditemukan',
    showRefresh: false,
    showExport: false,
    exportFilename: 'data',
    itemsPerPageOptions: () => [10, 25, 50, 100],
    defaultItemsPerPage: 10
});

const emit = defineEmits<{
    refresh: [];
    export: [format: 'csv' | 'excel'];
    rowClick: [row: any];
    search: [query: string];
    sort: [column: string | null, direction: 'asc' | 'desc' | null];
    pageChange: [page: number];
}>();

// Local state for client-side functionality
const searchQuery = ref('');
const sortColumn = ref<string | null>(null);
const sortDirection = ref<'asc' | 'desc' | null>(null);
const showFilters = ref(false);
const currentPage = ref(1);
const itemsPerPage = ref(props.defaultItemsPerPage);

// Client-side filtering and sorting
const filteredData = computed(() => {
    let filtered = [...props.data];

    // Apply search filter
    if (searchQuery.value.trim()) {
        const query = searchQuery.value.toLowerCase().trim();
        filtered = filtered.filter(row => {
            return props.columns.some(column => {
                if (column.searchable === false) return false;
                const value = getCellValue(row, column);
                // Handle null/undefined values and ensure we have a string to search
                const searchValue = value != null ? String(value).toLowerCase() : '';
                return searchValue.includes(query);
            });
        });
    }

    // Apply sorting
    if (sortColumn.value && sortDirection.value) {
        filtered.sort((a, b) => {
            const aVal = getCellValue(a, { key: sortColumn.value! } as Column);
            const bVal = getCellValue(b, { key: sortColumn.value! } as Column);

            let comparison = 0;
            if (aVal < bVal) comparison = -1;
            if (aVal > bVal) comparison = 1;

            return sortDirection.value === 'desc' ? -comparison : comparison;
        });
    }

    return filtered;
});

// Check if using server-side pagination
const isServerSidePagination = computed(() => props.pagination !== undefined);

// Client-side pagination (only when not using server-side)
const paginatedData = computed(() => {
    if (isServerSidePagination.value) {
        return props.data; // Server already handles pagination
    }

    const start = (currentPage.value - 1) * itemsPerPage.value;
    const end = start + itemsPerPage.value;
    return filteredData.value.slice(start, end);
});

const totalPages = computed(() => {
    if (isServerSidePagination.value) {
        return props.pagination?.last_page || 1;
    }
    return Math.ceil(filteredData.value.length / itemsPerPage.value);
});

const paginationInfo = computed(() => {
    if (isServerSidePagination.value) {
        return {
            from: props.pagination?.from || 0,
            to: props.pagination?.to || 0,
            total: props.pagination?.total || 0
        };
    }

    const total = filteredData.value.length;
    const from = total === 0 ? 0 : (currentPage.value - 1) * itemsPerPage.value + 1;
    const to = Math.min(currentPage.value * itemsPerPage.value, total);

    return { from, to, total };
});

// Watch for data changes to reset pagination (only for client-side)
watch(() => props.data, () => {
    if (!isServerSidePagination.value) {
        currentPage.value = 1;
    }
});

watch(searchQuery, () => {
    if (isServerSidePagination.value) {
        emit('search', searchQuery.value);
    } else {
        currentPage.value = 1;
    }
});

watch(itemsPerPage, () => {
    if (!isServerSidePagination.value) {
        currentPage.value = 1;
    }
});

// Methods
const handleSort = (column: Column) => {
    if (!column.sortable && column.sortable !== undefined) return;

    if (isServerSidePagination.value) {
        // For server-side sorting
        if (sortColumn.value === column.key) {
            if (sortDirection.value === 'asc') {
                sortDirection.value = 'desc';
            } else if (sortDirection.value === 'desc') {
                sortDirection.value = null;
                sortColumn.value = null;
            } else {
                sortDirection.value = 'asc';
            }
        } else {
            sortColumn.value = column.key;
            sortDirection.value = 'asc';
        }
        emit('sort', sortColumn.value, sortDirection.value);
    } else {
        // For client-side sorting
        if (sortColumn.value === column.key) {
            if (sortDirection.value === 'asc') {
                sortDirection.value = 'desc';
            } else if (sortDirection.value === 'desc') {
                sortDirection.value = null;
                sortColumn.value = null;
            } else {
                sortDirection.value = 'asc';
            }
        } else {
            sortColumn.value = column.key;
            sortDirection.value = 'asc';
        }
    }
};

const getSortIcon = (column: Column) => {
    if (!column.sortable && column.sortable !== undefined) return null;

    if (sortColumn.value === column.key) {
        return sortDirection.value === 'asc' ? ChevronUp : ChevronDown;
    }
    return ChevronsUpDown;
};

const handleRefresh = () => {
    emit('refresh');
};

const handleRowClick = (row: any) => {
    emit('rowClick', row);
};

const getCellValue = (row: any, column: Column) => {
    const value = row[column.key];

    // For name column, also include email in searchable text
    if (column.key === 'name' && row.email) {
        const nameValue = value || '';
        const emailValue = row.email || '';
        return column.render ? column.render(value, row) : `${nameValue} ${emailValue}`;
    }

    return column.render ? column.render(value, row) : value;
};

// Pagination methods
const goToPage = (page: number) => {
    if (page >= 1 && page <= totalPages.value) {
        if (isServerSidePagination.value) {
            emit('pageChange', page);
        } else {
            currentPage.value = page;
        }
    }
};

const nextPage = () => {
    const currentPageNum = isServerSidePagination.value ? (props.pagination?.current_page || 1) : currentPage.value;
    if (currentPageNum < totalPages.value) {
        goToPage(currentPageNum + 1);
    }
};

const prevPage = () => {
    const currentPageNum = isServerSidePagination.value ? (props.pagination?.current_page || 1) : currentPage.value;
    if (currentPageNum > 1) {
        goToPage(currentPageNum - 1);
    }
};

const getVisiblePages = () => {
    const delta = 2;
    const range = [];
    const rangeWithDots = [];
    const currentPageNum = isServerSidePagination.value ? (props.pagination?.current_page || 1) : currentPage.value;

    for (let i = Math.max(2, currentPageNum - delta);
         i <= Math.min(totalPages.value - 1, currentPageNum + delta);
         i++) {
        range.push(i);
    }

    if (currentPageNum - delta > 2) {
        rangeWithDots.push(1, '...');
    } else {
        rangeWithDots.push(1);
    }

    rangeWithDots.push(...range);

    if (currentPageNum + delta < totalPages.value - 1) {
        rangeWithDots.push('...', totalPages.value);
    } else if (totalPages.value > 1) {
        rangeWithDots.push(totalPages.value);
    }

    return rangeWithDots;
};
</script>

<template>
    <div class="bg-card rounded-lg border border-border overflow-hidden">
        <!-- Header with search and actions -->
        <div class="p-4 border-b border-border bg-muted/30">
            <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between gap-4">
                <!-- Search -->
                <div v-if="props.searchable" class="flex-1 max-w-md">
                    <div class="relative">
                        <Search class="absolute left-3 top-1/2 transform -translate-y-1/2 h-4 w-4 text-muted-foreground" />
                        <input
                            v-model="searchQuery"
                            type="text"
                            placeholder="Cari data..."
                            class="w-full pl-10 pr-4 py-2 border border-border rounded-md bg-background text-foreground placeholder:text-muted-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                            :disabled="props.loading"
                        />
                    </div>
                </div>

                <!-- Actions -->
                <div class="flex items-center gap-2">
                    <button
                        v-if="props.showRefresh"
                        @click="handleRefresh"
                        :disabled="props.loading"
                        class="p-2 text-muted-foreground hover:text-foreground border border-border rounded-md hover:bg-muted transition-colors disabled:opacity-50"
                        title="Refresh Data"
                    >
                        <RefreshCw :class="['h-4 w-4', { 'animate-spin': props.loading }]" />
                    </button>

                    <div v-if="props.showExport" class="relative">
                        <button
                            class="p-2 text-muted-foreground hover:text-foreground border border-border rounded-md hover:bg-muted transition-colors"
                            title="Export Data"
                        >
                            <Download class="h-4 w-4" />
                        </button>
                    </div>

                    <button
                        @click="showFilters = !showFilters"
                        class="p-2 text-muted-foreground hover:text-foreground border border-border rounded-md hover:bg-muted transition-colors"
                        title="Filter"
                    >
                        <Filter class="h-4 w-4" />
                    </button>
                </div>
            </div>

            <!-- Filters Row -->
            <div v-if="showFilters" class="mt-4 pt-4 border-t border-border">
                <slot name="filters" />
            </div>
        </div>

        <!-- Table -->
        <div class="overflow-x-auto">
            <table class="w-full">
                <thead class="bg-muted/50">
                    <tr>
                        <th
                            v-for="column in props.columns"
                            :key="column.key"
                            :class="[
                                'px-6 py-3 text-xs font-medium text-muted-foreground uppercase tracking-wider',
                                column.align === 'center' ? 'text-center' :
                                column.align === 'right' ? 'text-right' : 'text-left',
                                column.sortable !== false && props.sortable ? 'cursor-pointer select-none hover:bg-muted/70' : ''
                            ]"
                            :style="{ width: column.width }"
                            @click="handleSort(column)"
                        >
                            <div class="flex items-center gap-1">
                                {{ column.label }}
                                <component
                                    v-if="column.sortable !== false && props.sortable"
                                    :is="getSortIcon(column)"
                                    class="h-3 w-3"
                                />
                            </div>
                        </th>
                    </tr>
                </thead>
                <tbody class="divide-y divide-border">
                    <!-- Loading State -->
                    <tr v-if="props.loading">
                        <td :colspan="props.columns.length" class="px-6 py-8 text-center">
                            <div class="flex items-center justify-center">
                                <RefreshCw class="h-6 w-6 animate-spin text-muted-foreground mr-2" />
                                <span class="text-muted-foreground">Memuat data...</span>
                            </div>
                        </td>
                    </tr>

                    <!-- Empty State -->
                    <tr v-else-if="paginatedData.length === 0">
                        <td :colspan="props.columns.length" class="px-6 py-8 text-center text-muted-foreground">
                            <component v-if="props.emptyIcon" :is="props.emptyIcon" class="mx-auto h-12 w-12 text-muted-foreground/50" />
                            <p class="mt-2">{{ props.emptyMessage }}</p>
                        </td>
                    </tr>

                    <!-- Data Rows -->
                    <tr
                        v-else
                        v-for="(row, index) in paginatedData"
                        :key="index"
                        class="hover:bg-muted/30 transition-colors cursor-pointer"
                        @click="handleRowClick(row)"
                    >
                        <td
                            v-for="column in props.columns"
                            :key="column.key"
                            :class="[
                                'px-6 py-4',
                                column.align === 'center' ? 'text-center' :
                                column.align === 'right' ? 'text-right' : 'text-left'
                            ]"
                        >
                            <slot
                                :name="`cell-${column.key}`"
                                :value="row[column.key]"
                                :row="row"
                                :column="column"
                            >
                                <span v-html="getCellValue(row, column)"></span>
                            </slot>
                        </td>
                    </tr>
                </tbody>
            </table>
        </div>

        <!-- Client-side Pagination -->
        <div v-if="totalPages > 1" class="bg-muted/30 px-6 py-3 border-t border-border">
            <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between gap-4">
                <!-- Pagination Info -->
                <div class="text-sm text-muted-foreground">
                    Menampilkan {{ paginationInfo.from }} - {{ paginationInfo.to }} dari {{ paginationInfo.total }} data
                </div>

                <!-- Page Size Selector -->
                <div class="flex items-center gap-2 text-sm text-muted-foreground">
                    <span>Tampilkan:</span>
                    <select
                        v-model="itemsPerPage"
                        class="px-2 py-1 border border-border rounded bg-background text-foreground"
                    >
                        <option v-for="option in props.itemsPerPageOptions" :key="option" :value="option">
                            {{ option }}
                        </option>
                    </select>
                    <span>per halaman</span>
                </div>

                <!-- Pagination Buttons -->
                <div class="flex items-center space-x-1">
                    <!-- First Page -->
                    <button
                        @click="goToPage(1)"
                        :disabled="(isServerSidePagination ? (props.pagination?.current_page || 1) : currentPage) === 1"
                        :class="[
                            'p-2 rounded-md transition-colors',
                            (isServerSidePagination ? (props.pagination?.current_page || 1) : currentPage) === 1
                                ? 'text-muted-foreground/50 cursor-not-allowed'
                                : 'text-muted-foreground hover:bg-muted hover:text-foreground'
                        ]"
                        title="Halaman Pertama"
                    >
                        <ChevronsLeft class="h-4 w-4" />
                    </button>

                    <!-- Previous Page -->
                    <button
                        @click="prevPage"
                        :disabled="(isServerSidePagination ? (props.pagination?.current_page || 1) : currentPage) === 1"
                        :class="[
                            'px-3 py-2 text-sm rounded-md transition-colors',
                            (isServerSidePagination ? (props.pagination?.current_page || 1) : currentPage) === 1
                                ? 'text-muted-foreground/50 cursor-not-allowed'
                                : 'text-muted-foreground hover:bg-muted hover:text-foreground'
                        ]"
                    >
                        <ChevronLeft class="h-4 w-4" />
                    </button>

                    <!-- Page Numbers -->
                    <template v-for="page in getVisiblePages()" :key="page">
                        <span v-if="page === '...'" class="px-3 py-2 text-sm text-muted-foreground">...</span>
                        <button
                            v-else
                            @click="goToPage(Number(page))"
                            :class="[
                                'px-3 py-2 text-sm rounded-md transition-colors',
                                (isServerSidePagination ? (props.pagination?.current_page || 1) : currentPage) === page
                                    ? 'bg-primary text-primary-foreground'
                                    : 'text-muted-foreground hover:bg-muted hover:text-foreground'
                            ]"
                        >
                            {{ page }}
                        </button>
                    </template>

                    <!-- Next Page -->
                    <button
                        @click="nextPage"
                        :disabled="(isServerSidePagination ? (props.pagination?.current_page || 1) : currentPage) === totalPages"
                        :class="[
                            'px-3 py-2 text-sm rounded-md transition-colors',
                            (isServerSidePagination ? (props.pagination?.current_page || 1) : currentPage) === totalPages
                                ? 'text-muted-foreground/50 cursor-not-allowed'
                                : 'text-muted-foreground hover:bg-muted hover:text-foreground'
                        ]"
                    >
                        <ChevronRight class="h-4 w-4" />
                    </button>

                    <!-- Last Page -->
                    <button
                        @click="goToPage(totalPages)"
                        :disabled="(isServerSidePagination ? (props.pagination?.current_page || 1) : currentPage) === totalPages"
                        :class="[
                            'p-2 rounded-md transition-colors',
                            (isServerSidePagination ? (props.pagination?.current_page || 1) : currentPage) === totalPages
                                ? 'text-muted-foreground/50 cursor-not-allowed'
                                : 'text-muted-foreground hover:bg-muted hover:text-foreground'
                        ]"
                        title="Halaman Terakhir"
                    >
                        <ChevronsRight class="h-4 w-4" />
                    </button>
                </div>
            </div>
        </div>
    </div>
</template>
