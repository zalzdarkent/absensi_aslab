<script setup lang="ts">
import AppLayout from '@/layouts/AppLayout.vue';
import { type BreadcrumbItem } from '@/types';
import { Head, Link, useForm } from '@inertiajs/vue3';
import { ArrowLeft, Save } from 'lucide-vue-next';

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
        title: 'Tambah Aslab',
        href: '/aslabs/create',
    },
];

const form = useForm({
    name: '',
    email: '',
    password: '',
    password_confirmation: '',
    rfid_code: '',
    prodi: '',
    semester: 1,
});

const prodis = [
    'Teknik Informatika',
    'Sistem Informasi',
    'Teknik Komputer',
    'Teknologi Informasi',
    'Ilmu Komputer',
];

const submit = () => {
    form.post('/aslabs', {
        onSuccess: () => {
            // Redirect handled by controller
        },
    });
};
</script>

<template>
    <Head title="Tambah Aslab" />

    <AppLayout :breadcrumbs="breadcrumbs">
        <div class="flex h-full flex-1 flex-col gap-6 rounded-xl p-6 overflow-x-auto">
            <!-- Header -->
            <div class="flex items-center space-x-4">
                <Link
                    href="/aslabs"
                    class="p-2 bg-muted rounded-lg hover:bg-muted/80 transition-colors"
                >
                    <ArrowLeft class="h-5 w-5 text-muted-foreground" />
                </Link>
                <div>
                    <h1 class="text-2xl font-bold text-foreground">Tambah Aslab Baru</h1>
                    <p class="mt-1 text-sm text-muted-foreground">Tambahkan asisten laboratorium baru ke sistem</p>
                </div>
            </div>

            <!-- Form -->
            <div class="bg-card rounded-lg border border-border">
                <form @submit.prevent="submit" class="p-6 space-y-6">
                    <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
                        <!-- Nama -->
                        <div>
                            <label for="name" class="block text-sm font-medium text-foreground mb-2">
                                Nama Lengkap <span class="text-red-500">*</span>
                            </label>
                            <input
                                id="name"
                                v-model="form.name"
                                type="text"
                                required
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground placeholder:text-muted-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                                placeholder="Masukkan nama lengkap"
                            />
                            <div v-if="form.errors.name" class="mt-1 text-sm text-red-600">
                                {{ form.errors.name }}
                            </div>
                        </div>

                        <!-- Email -->
                        <div>
                            <label for="email" class="block text-sm font-medium text-foreground mb-2">
                                Email <span class="text-red-500">*</span>
                            </label>
                            <input
                                id="email"
                                v-model="form.email"
                                type="email"
                                required
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground placeholder:text-muted-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                                placeholder="masukkan@email.com"
                            />
                            <div v-if="form.errors.email" class="mt-1 text-sm text-red-600">
                                {{ form.errors.email }}
                            </div>
                        </div>

                        <!-- Password -->
                        <div>
                            <label for="password" class="block text-sm font-medium text-foreground mb-2">
                                Password <span class="text-red-500">*</span>
                            </label>
                            <input
                                id="password"
                                v-model="form.password"
                                type="password"
                                required
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground placeholder:text-muted-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                                placeholder="Minimal 8 karakter"
                            />
                            <div v-if="form.errors.password" class="mt-1 text-sm text-red-600">
                                {{ form.errors.password }}
                            </div>
                        </div>

                        <!-- Konfirmasi Password -->
                        <div>
                            <label for="password_confirmation" class="block text-sm font-medium text-foreground mb-2">
                                Konfirmasi Password <span class="text-red-500">*</span>
                            </label>
                            <input
                                id="password_confirmation"
                                v-model="form.password_confirmation"
                                type="password"
                                required
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground placeholder:text-muted-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                                placeholder="Ulangi password"
                            />
                        </div>

                        <!-- RFID Code -->
                        <div>
                            <label for="rfid_code" class="block text-sm font-medium text-foreground mb-2">
                                Kode RFID <span class="text-red-500">*</span>
                            </label>
                            <input
                                id="rfid_code"
                                v-model="form.rfid_code"
                                type="text"
                                required
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground placeholder:text-muted-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                                placeholder="RFID001"
                            />
                            <div v-if="form.errors.rfid_code" class="mt-1 text-sm text-red-600">
                                {{ form.errors.rfid_code }}
                            </div>
                        </div>

                        <!-- Program Studi -->
                        <div>
                            <label for="prodi" class="block text-sm font-medium text-foreground mb-2">
                                Program Studi <span class="text-red-500">*</span>
                            </label>
                            <select
                                id="prodi"
                                v-model="form.prodi"
                                required
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                            >
                                <option value="">Pilih Program Studi</option>
                                <option v-for="prodi in prodis" :key="prodi" :value="prodi">
                                    {{ prodi }}
                                </option>
                            </select>
                            <div v-if="form.errors.prodi" class="mt-1 text-sm text-red-600">
                                {{ form.errors.prodi }}
                            </div>
                        </div>

                        <!-- Semester -->
                        <div>
                            <label for="semester" class="block text-sm font-medium text-foreground mb-2">
                                Semester <span class="text-red-500">*</span>
                            </label>
                            <select
                                id="semester"
                                v-model="form.semester"
                                required
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                            >
                                <option v-for="i in 14" :key="i" :value="i">
                                    Semester {{ i }}
                                </option>
                            </select>
                            <div v-if="form.errors.semester" class="mt-1 text-sm text-red-600">
                                {{ form.errors.semester }}
                            </div>
                        </div>
                    </div>

                    <!-- Submit Button -->
                    <div class="flex items-center justify-end space-x-4 pt-6 border-t border-border">
                        <Link
                            href="/aslabs"
                            class="px-4 py-2 text-sm font-medium text-muted-foreground border border-border rounded-md hover:bg-muted transition-colors"
                        >
                            Batal
                        </Link>
                        <button
                            type="submit"
                            :disabled="form.processing"
                            class="inline-flex items-center px-4 py-2 bg-primary text-primary-foreground text-sm font-medium rounded-md hover:bg-primary/90 disabled:opacity-50 disabled:cursor-not-allowed transition-colors"
                        >
                            <Save class="h-4 w-4 mr-2" />
                            {{ form.processing ? 'Menyimpan...' : 'Simpan Aslab' }}
                        </button>
                    </div>
                </form>
            </div>
        </div>
    </AppLayout>
</template>
