<script setup lang="ts">
import AppLayout from '@/layouts/AppLayout.vue';
import { type BreadcrumbItem } from '@/types';
import { Head } from '@inertiajs/vue3';
import { Users, Clock, CheckCircle, XCircle, TrendingUp } from 'lucide-vue-next';

interface Attendance {
    user: {
        id: number;
        name: string;
        prodi: string;
        semester: number;
    };
    check_in: string | null;
    check_out: string | null;
    status: string;
}

interface Stats {
    total_aslabs: number;
    today_checkins: number;
    today_checkouts: number;
    active_today: number;
}

interface MostActiveAslab {
    name: string;
    prodi: string;
    semester: number;
    total_attendance: number;
}

interface WeeklyData {
    date: string;
    count: number;
}

interface Props {
    stats: Stats;
    today_attendances: Attendance[];
    most_active_aslabs: MostActiveAslab[];
    weekly_chart_data: WeeklyData[];
    current_date: string;
}

defineProps<Props>();

const breadcrumbs: BreadcrumbItem[] = [
    {
        title: 'Dashboard',
        href: '/dashboard',
    },
];

const getStatusColor = (status: string) => {
    switch (status) {
        case 'Sedang di Lab':
            return 'text-green-700 bg-green-100 dark:text-green-400 dark:bg-green-900/20';
        case 'Sudah Pulang':
            return 'text-blue-700 bg-blue-100 dark:text-blue-400 dark:bg-blue-900/20';
        default:
            return 'text-muted-foreground bg-muted';
    }
};
</script>

<template>
    <Head title="Dashboard Absensi Aslab" />

    <AppLayout :breadcrumbs="breadcrumbs">
        <div class="flex h-full flex-1 flex-col gap-6 rounded-xl p-6 overflow-x-auto">
            <!-- Header -->
            <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between">
                <div>
                    <h1 class="text-2xl font-bold text-foreground">Dashboard Absensi Aslab</h1>
                    <p class="mt-1 text-sm text-muted-foreground">{{ current_date }}</p>
                </div>
            </div>

            <!-- Statistics Cards -->
            <div class="grid grid-cols-1 gap-4 sm:grid-cols-2 lg:grid-cols-4">
                <div class="bg-card overflow-hidden shadow-sm rounded-lg border border-border">
                    <div class="p-5">
                        <div class="flex items-center">
                            <div class="flex-shrink-0">
                                <Users class="h-6 w-6 text-muted-foreground" />
                            </div>
                            <div class="ml-5 w-0 flex-1">
                                <dl>
                                    <dt class="text-sm font-medium text-muted-foreground truncate">Total Aslab Aktif</dt>
                                    <dd class="text-lg font-medium text-foreground">{{ stats.total_aslabs }}</dd>
                                </dl>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="bg-card overflow-hidden shadow-sm rounded-lg border border-border">
                    <div class="p-5">
                        <div class="flex items-center">
                            <div class="flex-shrink-0">
                                <CheckCircle class="h-6 w-6 text-green-500 dark:text-green-400" />
                            </div>
                            <div class="ml-5 w-0 flex-1">
                                <dl>
                                    <dt class="text-sm font-medium text-muted-foreground truncate">Check-in Hari Ini</dt>
                                    <dd class="text-lg font-medium text-foreground">{{ stats.today_checkins }}</dd>
                                </dl>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="bg-card overflow-hidden shadow-sm rounded-lg border border-border">
                    <div class="p-5">
                        <div class="flex items-center">
                            <div class="flex-shrink-0">
                                <XCircle class="h-6 w-6 text-red-500 dark:text-red-400" />
                            </div>
                            <div class="ml-5 w-0 flex-1">
                                <dl>
                                    <dt class="text-sm font-medium text-muted-foreground truncate">Check-out Hari Ini</dt>
                                    <dd class="text-lg font-medium text-foreground">{{ stats.today_checkouts }}</dd>
                                </dl>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="bg-card overflow-hidden shadow-sm rounded-lg border border-border">
                    <div class="p-5">
                        <div class="flex items-center">
                            <div class="flex-shrink-0">
                                <Clock class="h-6 w-6 text-indigo-500 dark:text-indigo-400" />
                            </div>
                            <div class="ml-5 w-0 flex-1">
                                <dl>
                                    <dt class="text-sm font-medium text-muted-foreground truncate">Sedang di Lab</dt>
                                    <dd class="text-lg font-medium text-foreground">{{ stats.active_today }}</dd>
                                </dl>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <div class="grid grid-cols-1 gap-6 lg:grid-cols-2">
                <!-- Today's Attendance -->
                <div class="bg-card shadow-sm rounded-lg border border-border">
                    <div class="p-6">
                        <h3 class="text-lg font-medium text-foreground mb-4">Absensi Hari Ini</h3>
                        <div class="overflow-hidden">
                            <div class="max-h-96 overflow-y-auto">
                                <div v-if="today_attendances.length === 0" class="text-center py-8">
                                    <Clock class="mx-auto h-12 w-12 text-muted-foreground" />
                                    <p class="mt-2 text-sm text-muted-foreground">Belum ada absensi hari ini</p>
                                </div>
                                <div v-else class="space-y-3">
                                    <div v-for="attendance in today_attendances" :key="attendance.user.id" class="flex items-center justify-between p-3 border border-border rounded-lg bg-background/50">
                                        <div class="flex-1">
                                            <p class="text-sm font-medium text-foreground">{{ attendance.user.name }}</p>
                                            <p class="text-xs text-muted-foreground">{{ attendance.user.prodi }} - Semester {{ attendance.user.semester }}</p>
                                        </div>
                                        <div class="flex items-center space-x-2">
                                            <div v-if="attendance.check_in" class="text-xs">
                                                <span class="text-muted-foreground">Masuk:</span>
                                                <span class="font-medium text-foreground">{{ attendance.check_in }}</span>
                                            </div>
                                            <div v-if="attendance.check_out" class="text-xs">
                                                <span class="text-muted-foreground">Pulang:</span>
                                                <span class="font-medium text-foreground">{{ attendance.check_out }}</span>
                                            </div>
                                            <span :class="['inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium', getStatusColor(attendance.status)]">
                                                {{ attendance.status }}
                                            </span>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

                <!-- Most Active Aslabs -->
                <div class="bg-card shadow-sm rounded-lg border border-border">
                    <div class="p-6">
                        <div class="flex items-center mb-4">
                            <TrendingUp class="h-5 w-5 text-muted-foreground mr-2" />
                            <h3 class="text-lg font-medium text-foreground">Aslab Paling Aktif Bulan Ini</h3>
                        </div>
                        <div class="overflow-hidden">
                            <div class="max-h-96 overflow-y-auto">
                                <div v-if="most_active_aslabs.length === 0" class="text-center py-8">
                                    <Users class="mx-auto h-12 w-12 text-muted-foreground" />
                                    <p class="mt-2 text-sm text-muted-foreground">Belum ada data absensi bulan ini</p>
                                </div>
                                <div v-else class="space-y-3">
                                    <div v-for="(aslab, index) in most_active_aslabs" :key="aslab.name" class="flex items-center justify-between p-3 border border-border rounded-lg bg-background/50">
                                        <div class="flex items-center space-x-3">
                                            <div class="flex-shrink-0">
                                                <div class="w-8 h-8 bg-primary/10 rounded-full flex items-center justify-center">
                                                    <span class="text-sm font-medium text-primary">{{ index + 1 }}</span>
                                                </div>
                                            </div>
                                            <div class="flex-1">
                                                <p class="text-sm font-medium text-foreground">{{ aslab.name }}</p>
                                                <p class="text-xs text-muted-foreground">{{ aslab.prodi }} - Semester {{ aslab.semester }}</p>
                                            </div>
                                        </div>
                                        <div class="flex items-center space-x-2">
                                            <span class="inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium bg-green-100 text-green-800 dark:bg-green-900/20 dark:text-green-400">
                                                {{ aslab.total_attendance }} hari
                                            </span>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Weekly Chart -->
            <div class="bg-card shadow-sm rounded-lg border border-border">
                <div class="p-6">
                    <h3 class="text-lg font-medium text-foreground mb-4">Grafik Kehadiran 7 Hari Terakhir</h3>
                    <div class="flex items-end space-x-2 h-64">
                        <div v-for="data in weekly_chart_data" :key="data.date" class="flex flex-col items-center flex-1">
                            <div class="bg-primary/20 rounded-t-md w-full flex items-end justify-center relative hover:bg-primary/30 transition-colors" :style="{ height: `${Math.max((data.count / Math.max(...weekly_chart_data.map(d => d.count))) * 200, 20)}px` }">
                                <span class="text-xs font-medium text-primary mb-1">{{ data.count }}</span>
                            </div>
                            <span class="text-xs text-muted-foreground mt-2">{{ data.date }}</span>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </AppLayout>
</template>
