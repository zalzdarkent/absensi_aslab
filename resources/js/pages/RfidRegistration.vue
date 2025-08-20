<template>
  <AppLayout>
    <div class="min-h-screen bg-gradient-to-br from-blue-50 via-indigo-50 to-purple-50 dark:from-gray-900 dark:via-slate-900 dark:to-gray-800">
      <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
        <!-- Header Section -->
        <div class="text-center mb-8">
          <div class="inline-flex items-center justify-center w-16 h-16 bg-gradient-to-r from-blue-500 to-purple-600 dark:from-blue-600 dark:to-purple-700 rounded-full mb-4">
            <svg class="w-8 h-8 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M3 10h18M7 15h1m4 0h1m-7 4h12a3 3 0 003-3V8a3 3 0 00-3-3H6a3 3 0 00-3 3v8a3 3 0 003 3z" />
            </svg>
          </div>
          <h1 class="text-4xl font-bold text-gray-900 dark:text-white mb-2">
            <span class="bg-gradient-to-r from-blue-600 to-purple-600 dark:from-blue-400 dark:to-purple-400 bg-clip-text text-transparent">
              Registrasi RFID Card
            </span>
          </h1>
          <p class="text-lg text-gray-600 dark:text-gray-300 max-w-2xl mx-auto">
            Daftarkan kartu RFID untuk asisten laboratorium dengan mudah dan cepat
          </p>
        </div>

        <!-- Alert Messages -->
        <div v-if="$page.props.flash && ($page.props.flash as any).success" class="mb-8 max-w-4xl mx-auto">
          <div class="rounded-lg bg-gradient-to-r from-green-50 to-emerald-50 dark:from-green-900 dark:to-emerald-900 border border-green-200 dark:border-green-700 p-4 shadow-sm">
            <div class="flex">
              <div class="flex-shrink-0">
                <div class="w-8 h-8 rounded-full bg-green-100 dark:bg-green-800 flex items-center justify-center">
                  <svg class="h-5 w-5 text-green-600 dark:text-green-300" viewBox="0 0 20 20" fill="currentColor">
                    <path fill-rule="evenodd" d="M10 18a8 8 0 100-16 8 8 0 000 16zm3.857-9.809a.75.75 0 00-1.214-.882l-3.236 4.53L7.53 10.23a.75.75 0 00-1.06 1.061l2.5 2.5a.75.75 0 001.137-.089l4-5.5z" clip-rule="evenodd" />
                  </svg>
                </div>
              </div>
              <div class="ml-3">
                <h3 class="text-sm font-medium text-green-800 dark:text-green-200">Berhasil!</h3>
                <p class="text-sm text-green-700 dark:text-green-300 mt-1">{{ ($page.props.flash as any).success }}</p>
              </div>
            </div>
          </div>
        </div>

        <!-- Form Registrasi -->
        <div class="max-w-4xl mx-auto mb-8">
          <div class="bg-white dark:bg-gray-800 rounded-2xl shadow-xl border border-gray-100 dark:border-gray-700 overflow-hidden">
            <div class="bg-gradient-to-r from-blue-500 to-purple-600 dark:from-blue-600 dark:to-purple-700 px-6 py-4">
              <div class="flex items-center">
                <div class="flex-shrink-0">
                  <div class="w-10 h-10 bg-white bg-opacity-20 rounded-lg flex items-center justify-center">
                    <svg class="w-6 h-6 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                      <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12h6m-6 4h6m2 5H7a2 2 0 01-2-2V5a2 2 0 012-2h5.586a1 1 0 01.707.293l5.414 5.414a1 1 0 01.293.707V19a2 2 0 01-2 2z" />
                    </svg>
                  </div>
                </div>
                <div class="ml-4">
                  <h3 class="text-xl font-semibold text-white">Form Registrasi RFID</h3>
                  <p class="text-blue-100 dark:text-blue-200 text-sm">Pilih asisten laboratorium dan scan kartu RFID untuk mendaftarkan</p>
                </div>
              </div>
            </div>

            <div class="p-6 dark:bg-gray-800">
              <form @submit.prevent="registerRfid" class="space-y-6">
                <div class="grid grid-cols-1 gap-6 lg:grid-cols-2">
                  <!-- Pilih Aslab -->
                  <div class="space-y-2">
                    <label for="user_id" class="block text-sm font-semibold text-gray-700 dark:text-gray-300">
                      <span class="flex items-center">
                        <svg class="w-4 h-4 mr-2 text-indigo-500 dark:text-indigo-400" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M16 7a4 4 0 11-8 0 4 4 0 018 0zM12 14a7 7 0 00-7 7h14a7 7 0 00-7-7z" />
                        </svg>
                        Pilih Asisten Laboratorium
                      </span>
                    </label>
                    <select
                      v-model="form.user_id"
                      id="user_id"
                      class="w-full rounded-xl border-2 border-gray-200 dark:border-gray-600 bg-gray-50 dark:bg-gray-700 px-4 py-3 text-sm text-gray-900 dark:text-white focus:border-indigo-500 dark:focus:border-indigo-400 focus:bg-white dark:focus:bg-gray-600 focus:ring-2 focus:ring-indigo-200 dark:focus:ring-indigo-300 transition-all duration-200"
                      required
                    >
                      <option value="">-- Pilih Aslab --</option>
                      <option
                        v-for="user in availableUsers"
                        :key="user.id"
                        :value="user.id"
                      >
                        {{ user.name }} ({{ user.prodi }} - Sem {{ user.semester }})
                      </option>
                    </select>
                    <p v-if="form.errors.user_id" class="text-sm text-red-600 dark:text-red-400 flex items-center">
                      <svg class="w-4 h-4 mr-1" fill="currentColor" viewBox="0 0 20 20">
                        <path fill-rule="evenodd" d="M18 10a8 8 0 11-16 0 8 8 0 0116 0zm-7 4a1 1 0 11-2 0 1 1 0 012 0zm-1-9a1 1 0 00-1 1v4a1 1 0 102 0V6a1 1 0 00-1-1z" clip-rule="evenodd" />
                      </svg>
                      {{ form.errors.user_id }}
                    </p>
                  </div>

                  <!-- RFID Code Input -->
                  <div class="space-y-2">
                    <label for="rfid_code" class="block text-sm font-semibold text-gray-700 dark:text-gray-300">
                      <span class="flex items-center">
                        <svg class="w-4 h-4 mr-2 text-purple-500 dark:text-purple-400" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M3 10h18M7 15h1m4 0h1m-7 4h12a3 3 0 003-3V8a3 3 0 00-3-3H6a3 3 0 00-3 3v8a3 3 0 003 3z" />
                        </svg>
                        Kode RFID
                      </span>
                    </label>
                    <div class="relative">
                      <input
                        v-model="form.rfid_code"
                        type="text"
                        id="rfid_code"
                        placeholder="Scan kartu RFID..."
                        class="w-full rounded-xl border-2 border-gray-200 dark:border-gray-600 bg-gray-50 dark:bg-gray-700 px-4 py-3 pr-12 text-sm text-gray-900 dark:text-white placeholder-gray-500 dark:placeholder-gray-400 focus:border-purple-500 dark:focus:border-purple-400 focus:bg-white dark:focus:bg-gray-600 focus:ring-2 focus:ring-purple-200 dark:focus:ring-purple-300 transition-all duration-200"
                        readonly
                      />
                      <div class="absolute inset-y-0 right-0 flex items-center pr-4">
                        <div v-if="scanningStatus === 'scanning'" class="flex items-center">
                          <div class="w-5 h-5 border-2 border-blue-500 dark:border-blue-400 border-t-transparent rounded-full animate-spin"></div>
                        </div>
                        <div v-else-if="scanningStatus === 'success'" class="w-6 h-6 bg-green-100 dark:bg-green-800 rounded-full flex items-center justify-center">
                          <svg class="w-4 h-4 text-green-600 dark:text-green-300" fill="currentColor" viewBox="0 0 20 20">
                            <path fill-rule="evenodd" d="M16.707 5.293a1 1 0 010 1.414l-8 8a1 1 0 01-1.414 0l-4-4a1 1 0 011.414-1.414L8 12.586l7.293-7.293a1 1 0 011.414 0z" clip-rule="evenodd" />
                          </svg>
                        </div>
                        <div v-else class="w-6 h-6 bg-gray-100 dark:bg-gray-600 rounded-full flex items-center justify-center">
                          <svg class="w-4 h-4 text-gray-400 dark:text-gray-500" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M3 10h18M7 15h1m4 0h1m-7 4h12a3 3 0 003-3V8a3 3 0 00-3-3H6a3 3 0 00-3 3v8a3 3 0 003 3z" />
                          </svg>
                        </div>
                      </div>
                    </div>
                    <div class="flex items-center space-x-2 text-sm">
                      <div v-if="scanningStatus === 'scanning'" class="flex items-center text-blue-600 dark:text-blue-400">
                        <div class="w-2 h-2 bg-blue-500 dark:bg-blue-400 rounded-full animate-pulse mr-2"></div>
                        <span class="font-medium">Menunggu scan kartu RFID...</span>
                      </div>
                      <div v-else-if="scanningStatus === 'success'" class="flex items-center text-green-600 dark:text-green-400">
                        <div class="w-2 h-2 bg-green-500 dark:bg-green-400 rounded-full mr-2"></div>
                        <span class="font-medium">Kartu berhasil di-scan!</span>
                      </div>
                      <div v-else class="flex items-center text-gray-500 dark:text-gray-400">
                        <div class="w-2 h-2 bg-gray-400 dark:bg-gray-500 rounded-full mr-2"></div>
                        <span>Klik "Mulai Scan" lalu tempelkan kartu RFID</span>
                      </div>
                    </div>
                    <p v-if="form.errors.rfid_code" class="text-sm text-red-600 dark:text-red-400 flex items-center">
                      <svg class="w-4 h-4 mr-1" fill="currentColor" viewBox="0 0 20 20">
                        <path fill-rule="evenodd" d="M18 10a8 8 0 11-16 0 8 8 0 0116 0zm-7 4a1 1 0 11-2 0 1 1 0 012 0zm-1-9a1 1 0 00-1 1v4a1 1 0 102 0V6a1 1 0 00-1-1z" clip-rule="evenodd" />
                      </svg>
                      {{ form.errors.rfid_code }}
                    </p>
                  </div>
                </div>

                <!-- Action Buttons -->
                <div class="flex flex-col sm:flex-row items-center justify-between space-y-3 sm:space-y-0 sm:space-x-4 pt-6 border-t border-gray-100 dark:border-gray-700">
                  <button
                    type="button"
                    @click="startScanning"
                    :disabled="scanningStatus === 'scanning'"
                    class="w-full sm:w-auto inline-flex items-center justify-center rounded-xl bg-gradient-to-r from-blue-500 to-blue-600 dark:from-blue-600 dark:to-blue-700 px-6 py-3 text-sm font-semibold text-white shadow-lg hover:from-blue-600 hover:to-blue-700 dark:hover:from-blue-700 dark:hover:to-blue-800 focus:outline-none focus:ring-2 focus:ring-blue-500 dark:focus:ring-blue-400 focus:ring-offset-2 dark:focus:ring-offset-gray-800 disabled:opacity-50 disabled:cursor-not-allowed transform transition-all duration-200 hover:scale-105"
                  >
                    <svg v-if="scanningStatus === 'scanning'" class="animate-spin -ml-1 mr-2 h-4 w-4" xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24">
                      <circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4"></circle>
                      <path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
                    </svg>
                    <svg v-else class="-ml-1 mr-2 h-4 w-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                      <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M3 10h18M7 15h1m4 0h1m-7 4h12a3 3 0 003-3V8a3 3 0 00-3-3H6a3 3 0 00-3 3v8a3 3 0 003 3z" />
                    </svg>
                    {{ scanningStatus === 'scanning' ? 'Menunggu Scan...' : 'Mulai Scan RFID' }}
                  </button>

                  <button
                    type="submit"
                    :disabled="!form.rfid_code || !form.user_id || form.processing"
                    class="w-full sm:w-auto inline-flex items-center justify-center rounded-xl bg-gradient-to-r from-green-500 to-emerald-600 dark:from-green-600 dark:to-emerald-700 px-8 py-3 text-sm font-semibold text-white shadow-lg hover:from-green-600 hover:to-emerald-700 dark:hover:from-green-700 dark:hover:to-emerald-800 focus:outline-none focus:ring-2 focus:ring-green-500 dark:focus:ring-green-400 focus:ring-offset-2 dark:focus:ring-offset-gray-800 disabled:opacity-50 disabled:cursor-not-allowed transform transition-all duration-200 hover:scale-105"
                  >
                    <svg v-if="form.processing" class="animate-spin -ml-1 mr-2 h-4 w-4" xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24">
                      <circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4"></circle>
                      <path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
                    </svg>
                    <svg v-else class="-ml-1 mr-2 h-4 w-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                      <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M5 13l4 4L19 7" />
                    </svg>
                    {{ form.processing ? 'Mendaftarkan...' : 'Daftarkan RFID' }}
                  </button>
                </div>
              </form>
            </div>
          </div>
        </div>

        <!-- Daftar Aslab -->
        <div class="max-w-7xl mx-auto">
          <div class="bg-white dark:bg-gray-800 rounded-2xl shadow-xl border border-gray-100 dark:border-gray-700 overflow-hidden">
            <div class="bg-gradient-to-r from-purple-500 to-pink-600 dark:from-purple-600 dark:to-pink-700 px-6 py-6">
              <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between">
                <div class="flex items-center">
                  <div class="flex-shrink-0">
                    <div class="w-12 h-12 bg-white bg-opacity-20 rounded-xl flex items-center justify-center">
                      <svg class="w-7 h-7 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M17 20h5v-2a3 3 0 00-5.356-1.857M17 20H7m10 0v-2c0-.656-.126-1.283-.356-1.857M7 20H2v-2a3 3 0 015.356-1.857M7 20v-2c0-.656.126-1.283.356-1.857m0 0a5.002 5.002 0 019.288 0M15 7a3 3 0 11-6 0 3 3 0 016 0zm6 3a2 2 0 11-4 0 2 2 0 014 0zM7 10a2 2 0 11-4 0 2 2 0 014 0z" />
                      </svg>
                    </div>
                  </div>
                  <div class="ml-4">
                    <h3 class="text-xl font-semibold text-white">Daftar Asisten Laboratorium</h3>
                    <p class="text-purple-100 dark:text-purple-200 text-sm">Status registrasi RFID untuk setiap aslab</p>
                  </div>
                </div>
                <div class="mt-4 sm:mt-0">
                  <div class="flex items-center space-x-6">
                    <div class="flex items-center bg-green-500 dark:bg-green-600 rounded-lg px-4 py-2 shadow-md">
                      <div class="h-3 w-3 rounded-full bg-white mr-2"></div>
                      <span class="text-white text-sm font-bold">Terdaftar: {{ registeredCount }}</span>
                    </div>
                    <div class="flex items-center bg-red-500 dark:bg-red-600 rounded-lg px-4 py-2 shadow-md">
                      <div class="h-3 w-3 rounded-full bg-white mr-2"></div>
                      <span class="text-white text-sm font-bold">Belum: {{ unregisteredCount }}</span>
                    </div>
                  </div>
                </div>
              </div>
            </div>

            <div class="overflow-hidden">
              <div class="overflow-x-auto">
                <table class="min-w-full divide-y divide-gray-200 dark:divide-gray-600">
                  <thead class="bg-gradient-to-r from-gray-50 to-gray-100 dark:from-gray-700 dark:to-gray-600">
                    <tr>
                      <th scope="col" class="px-6 py-4 text-left text-xs font-bold text-gray-600 dark:text-gray-300 uppercase tracking-wider">
                        Aslab
                      </th>
                      <th scope="col" class="px-6 py-4 text-left text-xs font-bold text-gray-600 dark:text-gray-300 uppercase tracking-wider">
                        Program Studi
                      </th>
                      <th scope="col" class="px-6 py-4 text-left text-xs font-bold text-gray-600 dark:text-gray-300 uppercase tracking-wider">
                        Semester
                      </th>
                      <th scope="col" class="px-6 py-4 text-left text-xs font-bold text-gray-600 dark:text-gray-300 uppercase tracking-wider">
                        RFID Code
                      </th>
                      <th scope="col" class="px-6 py-4 text-left text-xs font-bold text-gray-600 dark:text-gray-300 uppercase tracking-wider">
                        Status
                      </th>
                    </tr>
                  </thead>
                  <tbody class="bg-white dark:bg-gray-800 divide-y divide-gray-100 dark:divide-gray-600">
                    <tr v-for="user in users" :key="user.id" class="hover:bg-gradient-to-r hover:from-blue-50 hover:to-purple-50 dark:hover:from-gray-700 dark:hover:to-gray-600 transition-all duration-200">
                      <td class="px-6 py-4">
                        <div class="flex items-center">
                          <div class="flex-shrink-0 h-12 w-12">
                            <div class="h-12 w-12 rounded-xl bg-gradient-to-br from-indigo-400 to-purple-500 dark:from-indigo-500 dark:to-purple-600 flex items-center justify-center shadow-md">
                              <span class="text-sm font-bold text-white">{{ user.name.split(' ').map(n => n[0]).join('').toUpperCase() }}</span>
                            </div>
                          </div>
                          <div class="ml-4">
                            <div class="text-sm font-semibold text-gray-900 dark:text-white">{{ user.name }}</div>
                            <div class="text-sm text-gray-500 dark:text-gray-400">{{ user.email }}</div>
                          </div>
                        </div>
                      </td>
                      <td class="px-6 py-4 whitespace-nowrap">
                        <div class="text-sm font-medium text-gray-900 dark:text-gray-200">{{ user.prodi }}</div>
                      </td>
                      <td class="px-6 py-4 whitespace-nowrap">
                        <div class="text-sm font-medium text-gray-900 dark:text-gray-200">Semester {{ user.semester }}</div>
                      </td>
                      <td class="px-6 py-4 whitespace-nowrap">
                        <code v-if="user.rfid_code" class="inline-flex items-center px-3 py-1 rounded-lg text-xs font-mono font-medium bg-gradient-to-r from-gray-100 to-gray-200 dark:from-gray-600 dark:to-gray-700 text-gray-800 dark:text-gray-200 border border-gray-300 dark:border-gray-500 shadow-sm">
                          {{ user.rfid_code }}
                        </code>
                        <span v-else class="text-gray-400 dark:text-gray-500 italic text-sm">Belum terdaftar</span>
                      </td>
                      <td class="px-6 py-4 whitespace-nowrap">
                        <span v-if="user.rfid_code" class="inline-flex items-center px-3 py-1 rounded-full text-xs font-bold bg-green-500 dark:bg-green-600 text-white border border-green-600 dark:border-green-500 shadow-md">
                          <svg class="-ml-0.5 mr-1.5 h-2 w-2 text-white" fill="currentColor" viewBox="0 0 8 8">
                            <circle cx="4" cy="4" r="3" />
                          </svg>
                          Terdaftar
                        </span>
                        <span v-else class="inline-flex items-center px-3 py-1 rounded-full text-xs font-bold bg-red-500 dark:bg-red-600 text-white border border-red-600 dark:border-red-500 shadow-md">
                          <svg class="-ml-0.5 mr-1.5 h-2 w-2 text-white" fill="currentColor" viewBox="0 0 8 8">
                            <circle cx="4" cy="4" r="3" />
                          </svg>
                          Belum Terdaftar
                        </span>
                      </td>
                    </tr>
                  </tbody>
                </table>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </AppLayout>
</template>

<script setup lang="ts">
import { useForm } from '@inertiajs/vue3'
import { ref, computed, onMounted, onUnmounted } from 'vue'
import AppLayout from '@/layouts/AppLayout.vue'

interface User {
  id: number
  name: string
  email: string
  prodi: string
  semester: number
  rfid_code: string | null
  is_active: boolean
}

interface Props {
  users: User[]
}

const props = defineProps<Props>()

const form = useForm({
  user_id: '',
  rfid_code: ''
})

const scanningStatus = ref<'idle' | 'scanning' | 'success'>('idle')
let scanInterval: number | null = null

// Filter users yang belum punya RFID
const availableUsers = computed(() => {
  return props.users.filter(user => !user.rfid_code)
})

// Count registered vs unregistered
const registeredCount = computed(() => {
  return props.users.filter(user => user.rfid_code).length
})

const unregisteredCount = computed(() => {
  return props.users.filter(user => !user.rfid_code).length
})

const startScanning = () => {
  if (scanningStatus.value === 'scanning') return

  scanningStatus.value = 'scanning'
  form.rfid_code = ''

  // Poll setiap 2 detik untuk cek RFID baru
  scanInterval = setInterval(async () => {
    try {
      const response = await fetch('/api/rfid/last-scan', {
        method: 'GET',
        headers: {
          'Content-Type': 'application/json',
          'X-CSRF-TOKEN': document.querySelector('meta[name="csrf-token"]')?.getAttribute('content') || ''
        }
      })

      if (response.ok) {
        const data = await response.json()
        if (data.success && data.data.rfid_code) {
          form.rfid_code = data.data.rfid_code
          scanningStatus.value = 'success'
          stopScanning()
        }
      }
    } catch (error) {
      console.error('Scanning error:', error)
    }
  }, 2000)

  // Auto stop setelah 30 detik
  setTimeout(() => {
    if (scanningStatus.value === 'scanning') {
      stopScanning()
    }
  }, 30000)
}

const stopScanning = () => {
  if (scanInterval) {
    clearInterval(scanInterval)
    scanInterval = null
  }
  if (scanningStatus.value === 'scanning') {
    scanningStatus.value = 'idle'
  }
}

const registerRfid = () => {
  form.post('/rfid-registration', {
    onSuccess: () => {
      form.reset()
      scanningStatus.value = 'idle'
      // Reload users data
      window.location.reload()
    }
  })
}

onMounted(() => {
  // Auto start scanning if no RFID detected yet
})

onUnmounted(() => {
  stopScanning()
})
</script>
