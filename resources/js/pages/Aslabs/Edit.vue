<script setup lang="ts">
import AppLayout from '@/layouts/AppLayout.vue';
import { type BreadcrumbItem } from '@/types';
import { Head, Link, useForm } from '@inertiajs/vue3';
import { ArrowLeft, Save, Trash2 } from 'lucide-vue-next';

interface User {
    id: number;
    name: string;
    email: string;
    rfid_code: string;
    prodi: string;
    semester: number;
    is_active: boolean;
}

interface Props {
    aslab: User;
}

const props = defineProps<Props>();

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
    {
        title: 'Edit',
        href: `/aslabs/${props.aslab.id}/edit`,
    },
];

const form = useForm({
    name: props.aslab.name,
    email: props.aslab.email,
    password: '',
    password_confirmation: '',
    rfid_code: props.aslab.rfid_code,
    prodi: props.aslab.prodi,
    semester: props.aslab.semester,
    is_active: props.aslab.is_active,
});

const prodis = [
    'Informatika',
    'Informasi',
];

const submit = () => {
    form.put(`/aslabs/${props.aslab.id}`, {
        onSuccess: () => {
            // Redirect handled by controller
        },
    });
};

const deleteAslab = () => {
    if (confirm('Apakah Anda yakin ingin menghapus aslab ini? Tindakan ini tidak dapat dibatalkan.')) {
        form.delete(`/aslabs/${props.aslab.id}`);
    }
};
</script>

<template>
    <Head :title="`Edit ${aslab.name}`" />

    <AppLayout :breadcrumbs="breadcrumbs">
        <div class="flex h-full flex-1 flex-col gap-6 rounded-xl p-6 overflow-x-auto">
            <!-- Header -->
            <div class="flex items-center justify-between">
                <div class="flex items-center space-x-4">
                    <Link
                        :href="`/aslabs/${aslab.id}`"
                        class="p-2 bg-muted rounded-lg hover:bg-muted/80 transition-colors"
                    >
                        <ArrowLeft class="h-5 w-5 text-muted-foreground" />
                    </Link>
                    <div>
                        <h1 class="text-2xl font-bold text-foreground">Edit {{ aslab.name }}</h1>
                        <p class="mt-1 text-sm text-muted-foreground">Perbarui informasi asisten laboratorium</p>
                    </div>
                </div>
                <button
                    @click="deleteAslab"
                    class="inline-flex items-center px-4 py-2 bg-red-600 text-white text-sm font-medium rounded-md hover:bg-red-700 transition-colors"
                >
                    <Trash2 class="h-4 w-4 mr-2" />
                    Hapus Aslab
                </button>
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
                                Password Baru
                            </label>
                            <input
                                id="password"
                                v-model="form.password"
                                type="password"
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground placeholder:text-muted-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                                placeholder="Kosongkan jika tidak ingin mengubah"
                            />
                            <div v-if="form.errors.password" class="mt-1 text-sm text-red-600">
                                {{ form.errors.password }}
                            </div>
                        </div>

                        <!-- Konfirmasi Password -->
                        <div>
                            <label for="password_confirmation" class="block text-sm font-medium text-foreground mb-2">
                                Konfirmasi Password Baru
                            </label>
                            <input
                                id="password_confirmation"
                                v-model="form.password_confirmation"
                                type="password"
                                class="w-full px-3 py-2 border border-border rounded-md bg-background text-foreground placeholder:text-muted-foreground focus:outline-none focus:ring-2 focus:ring-primary focus:border-transparent"
                                placeholder="Ulangi password baru"
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

                        <!-- Status -->
                        <div>
                            <label class="block text-sm font-medium text-foreground mb-2">
                                Status
                            </label>
                            <div class="flex items-center space-x-3">
                                <label class="flex items-center">
                                    <input
                                        v-model="form.is_active"
                                        type="checkbox"
                                        class="rounded border-border text-primary focus:ring-primary focus:ring-offset-0"
                                    />
                                    <span class="ml-2 text-sm text-foreground">Aslab Aktif</span>
                                </label>
                            </div>
                            <p class="mt-1 text-xs text-muted-foreground">
                                Aslab non-aktif tidak dapat melakukan absensi
                            </p>
                        </div>
                    </div>

                    <!-- Submit Button -->
                    <div class="flex items-center justify-end space-x-4 pt-6 border-t border-border">
                        <Link
                            :href="`/aslabs/${aslab.id}`"
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
                            {{ form.processing ? 'Menyimpan...' : 'Simpan Perubahan' }}
                        </button>
                    </div>
                </form>
            </div>
        </div>
    </AppLayout>
</template>
