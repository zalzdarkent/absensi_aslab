<template>
  <AppLayout>
    <!-- Toast Notifications -->
    <div class="fixed top-4 right-4 z-50 space-y-3">
      <div
        v-for="toast in toasts"
        :key="toast.id"
        class="flex items-start w-96 max-w-sm bg-card text-card-foreground border border-border rounded-lg shadow-lg transform transition-all duration-300 ease-in-out animate-in slide-in-from-right"
      >
        <div class="flex items-start p-4 w-full">
          <div class="flex-shrink-0">
            <CheckCircle v-if="toast.type === 'success'" class="h-5 w-5 text-green-500" />
            <XCircle v-else-if="toast.type === 'error'" class="h-5 w-5 text-red-500" />
            <Clock v-else class="h-5 w-5 text-yellow-500" />
          </div>
          <div class="ml-3 flex-1 min-w-0">
            <div class="text-sm font-semibold text-foreground">{{ toast.title }}</div>
            <div class="text-sm text-muted-foreground mt-1 break-words">{{ toast.message }}</div>
          </div>
          <button
            @click="removeToast(toast.id)"
            class="ml-3 flex-shrink-0 text-muted-foreground hover:text-foreground transition-colors duration-200"
          >
            <X class="h-4 w-4" />
          </button>
        </div>
      </div>
    </div>

    <div class="flex h-full flex-1 flex-col gap-6 rounded-xl p-6">
      <!-- Header -->
      <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between">
        <div>
          <h1 class="text-2xl font-bold text-foreground">Registrasi RFID Card</h1>
          <p class="mt-1 text-sm text-muted-foreground">Daftarkan kartu RFID untuk asisten laboratorium dengan mudah dan cepat</p>
        </div>
      </div>

      <!-- Alert Messages -->
      <div v-if="$page.props.flash && ($page.props.flash as any).success" class="mb-6">
        <div class="rounded-lg bg-green-50 dark:bg-green-900/20 border border-green-200 dark:border-green-700 p-4">
          <div class="flex">
            <div class="flex-shrink-0">
              <div class="w-8 h-8 rounded-full bg-green-100 dark:bg-green-800 flex items-center justify-center">
                <CheckCircle class="h-5 w-5 text-green-600 dark:text-green-300" />
              </div>
            </div>
            <div class="ml-3">
              <h3 class="text-sm font-medium text-green-800 dark:text-green-200">Berhasil!</h3>
              <p class="text-sm text-green-700 dark:text-green-300 mt-1">{{ ($page.props.flash as any).success }}</p>
            </div>
          </div>
        </div>
      </div>

      <!-- Form Registrasi RFID -->
      <div class="bg-card rounded-lg border border-border p-6 mb-6">
        <div class="flex items-center mb-6">
          <div class="w-10 h-10 bg-blue-500 rounded-lg flex items-center justify-center mr-4">
            <Users class="w-6 h-6 text-white" />
          </div>
          <div>
            <h3 class="text-xl font-semibold text-foreground">Form Registrasi RFID</h3>
            <p class="text-muted-foreground text-sm">Pilih asisten laboratorium dan scan kartu RFID untuk mendaftarkan</p>
          </div>
        </div>
        <form @submit.prevent="registerRfid" class="space-y-6">
          <div class="grid grid-cols-1 gap-6 lg:grid-cols-2">
            <!-- Pilih Aslab -->
            <div class="space-y-2">
              <label for="user_id" class="block text-sm font-medium text-foreground">
                Pilih Asisten Laboratorium
              </label>
              <select
                v-model="form.user_id"
                id="user_id"
                class="w-full rounded-lg border border-border bg-background px-3 py-2 text-sm text-foreground placeholder-muted-foreground focus:border-primary focus:outline-none focus:ring-2 focus:ring-primary/20"
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
              <p v-if="form.errors.user_id" class="text-sm text-destructive flex items-center">
                <XCircle class="w-4 h-4 mr-1" />
                {{ form.errors.user_id }}
              </p>
            </div>

            <!-- RFID Code Input -->
            <div class="space-y-2">
              <label for="rfid_code" class="block text-sm font-medium text-foreground">
                Kode RFID
              </label>
              <div class="relative">
                <input
                  v-model="form.rfid_code"
                  type="text"
                  id="rfid_code"
                  placeholder="Scan kartu RFID..."
                  class="w-full rounded-lg border border-border bg-background px-3 py-2 pr-10 text-sm text-foreground placeholder-muted-foreground focus:border-primary focus:outline-none focus:ring-2 focus:ring-primary/20"
                  readonly
                />
                <div class="absolute inset-y-0 right-0 flex items-center pr-3">
                  <div v-if="scanningStatus === 'scanning'" class="flex items-center">
                    <div class="w-4 h-4 border-2 border-primary border-t-transparent rounded-full animate-spin"></div>
                  </div>
                  <CheckCircle v-else-if="scanningStatus === 'success'" class="w-4 h-4 text-green-500" />
                  <Users v-else class="w-4 h-4 text-muted-foreground" />
                </div>
              </div>
              <div class="flex items-center space-x-2 text-sm">
                <div v-if="scanningStatus === 'scanning'" class="flex items-center text-blue-600 dark:text-blue-400">
                  <div class="w-2 h-2 bg-blue-500 rounded-full animate-pulse mr-2"></div>
                  <span class="font-medium">Menunggu scan kartu RFID...</span>
                </div>
                <div v-else-if="scanningStatus === 'success'" class="flex items-center text-green-600 dark:text-green-400">
                  <div class="w-2 h-2 bg-green-500 rounded-full mr-2"></div>
                  <span class="font-medium">Kartu berhasil di-scan!</span>
                </div>
                <div v-else class="flex items-center text-muted-foreground">
                  <div class="w-2 h-2 bg-muted-foreground rounded-full mr-2"></div>
                  <span>Klik "Mulai Scan" lalu tempelkan kartu RFID</span>
                </div>
              </div>
              <p v-if="form.errors.rfid_code" class="text-sm text-destructive flex items-center">
                <XCircle class="w-4 h-4 mr-1" />
                {{ form.errors.rfid_code }}
              </p>
            </div>
          </div>

          <!-- Action Buttons -->
          <div class="flex flex-col sm:flex-row items-center justify-between space-y-3 sm:space-y-0 sm:space-x-4 pt-6 border-t border-border">
            <div class="flex flex-col sm:flex-row space-y-3 sm:space-y-0 sm:space-x-3">
              <button
                type="button"
                @click="startScanning"
                :disabled="scanningStatus === 'scanning'"
                class="inline-flex items-center justify-center rounded-lg bg-blue-500 hover:bg-blue-600 px-4 py-2 text-sm font-medium text-white transition-colors focus:outline-none focus:ring-2 focus:ring-blue-500/20 disabled:opacity-50 disabled:cursor-not-allowed"
              >
                <Clock v-if="scanningStatus === 'scanning'" class="animate-spin -ml-1 mr-2 h-4 w-4" />
                <Users v-else class="-ml-1 mr-2 h-4 w-4" />
                {{ scanningStatus === 'scanning' ? 'Menunggu Scan...' : 'Mulai Scan RFID' }}
              </button>
            </div>

            <button
              type="submit"
              :disabled="!form.rfid_code || !form.user_id || form.processing"
              class="inline-flex items-center justify-center rounded-lg bg-green-500 hover:bg-green-600 px-6 py-2 text-sm font-medium text-white transition-colors focus:outline-none focus:ring-2 focus:ring-green-500/20 disabled:opacity-50 disabled:cursor-not-allowed"
            >
              <Clock v-if="form.processing" class="animate-spin -ml-1 mr-2 h-4 w-4" />
              <CheckCircle v-else class="-ml-1 mr-2 h-4 w-4" />
              {{ form.processing ? 'Mendaftarkan...' : 'Daftarkan RFID' }}
            </button>
          </div>
        </form>
      </div>

      <!-- Daftar Asisten Laboratorium -->
      <div class="bg-card rounded-lg border border-border p-6">
        <div class="flex items-center justify-between mb-6">
          <div class="flex items-center">
            <div class="w-10 h-10 bg-purple-500 rounded-lg flex items-center justify-center mr-4">
              <Users class="w-6 h-6 text-white" />
            </div>
            <div>
              <h3 class="text-xl font-semibold text-foreground">Daftar Asisten Laboratorium</h3>
              <p class="text-muted-foreground text-sm">Status registrasi RFID untuk setiap aslab</p>
            </div>
          </div>
          <div class="flex items-center space-x-4">
            <div class="flex items-center bg-green-500 rounded-lg px-3 py-1">
              <div class="h-2 w-2 rounded-full bg-white mr-2"></div>
              <span class="text-white text-xs font-medium">Terdaftar: {{ registeredCount }}</span>
            </div>
            <div class="flex items-center bg-red-500 rounded-lg px-3 py-1">
              <div class="h-2 w-2 rounded-full bg-white mr-2"></div>
              <span class="text-white text-xs font-medium">Belum: {{ unregisteredCount }}</span>
            </div>
          </div>
        </div>

        <!-- DataTable for Users -->
        <DataTable
          :columns="columns"
          :data="usersWithFlatProperties"
          :empty-message="'Belum ada asisten laboratorium'"
          :empty-icon="Users"
          :show-refresh="false"
          :show-export="false"
          :pagination="undefined"
          @search="handleSearch"
          @sort="handleSort"
          @row-click="handleRowClick"
        >
          <!-- Custom cell templates -->
          <template #cell-name="{ row }">
            <div class="flex items-center">
              <div class="flex-shrink-0 h-10 w-10">
                <div class="h-10 w-10 rounded-lg bg-gradient-to-br from-indigo-400 to-purple-500 flex items-center justify-center">
                  <span class="text-sm font-bold text-white">{{ row.name.split(' ').map((n: string) => n[0]).join('').toUpperCase() }}</span>
                </div>
              </div>
              <div class="ml-3">
                <div class="text-sm font-medium text-foreground">{{ row.name }}</div>
                <div class="text-sm text-muted-foreground">{{ row.email }}</div>
              </div>
            </div>
          </template>

          <template #cell-semester="{ row }">
            <span class="text-sm text-foreground">Semester {{ row.semester }}</span>
          </template>

          <template #cell-rfid_code="{ row }">
            <code v-if="row.rfid_code" class="inline-flex items-center px-2 py-1 rounded text-xs font-mono bg-muted border border-border text-foreground">
              {{ row.rfid_code }}
            </code>
            <span v-else class="text-muted-foreground italic text-sm">Belum terdaftar</span>
          </template>

          <template #cell-status="{ row }">
            <span v-if="row.rfid_code" class="inline-flex items-center px-2 py-1 rounded-full text-xs font-medium bg-green-100 text-green-800 dark:bg-green-900/20 dark:text-green-400">
              <CheckCircle class="h-3 w-3 mr-1" />
              Terdaftar
            </span>
            <span v-else class="inline-flex items-center px-2 py-1 rounded-full text-xs font-medium bg-red-100 text-red-800 dark:bg-red-900/20 dark:text-red-400">
              <XCircle class="h-3 w-3 mr-1" />
              Belum Terdaftar
            </span>
          </template>
        </DataTable>
      </div>
    </div>
  </AppLayout>
</template>

<script setup lang="ts">
import { useForm } from '@inertiajs/vue3'
import { ref, computed, onMounted, onUnmounted } from 'vue'
import AppLayout from '@/layouts/AppLayout.vue'
import DataTable, { type Column } from '@/components/DataTable.vue'
import { Users, CheckCircle, XCircle, Clock, X } from 'lucide-vue-next'

interface User {
  id: number
  name: string
  email: string
  prodi: string
  semester: number
  rfid_code: string | null
  is_active: boolean
}

interface Toast {
  id: number
  type: 'success' | 'error' | 'warning'
  title: string
  message: string
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
const toasts = ref<Toast[]>([])
let scanInterval: number | null = null
let toastIdCounter = 0

// Audio setup for success feedback
const successAudio = ref<HTMLAudioElement | null>(null)

// Initialize audio
const initializeAudio = () => {
  try {
    successAudio.value = new Audio('/audio/hidupjokowi.mp3')
    successAudio.value.volume = 0.7 // Set volume to 70%
    successAudio.value.preload = 'auto'
    
    // Test if audio can load
    successAudio.value.addEventListener('canplaythrough', () => {
      console.log('Success audio loaded successfully')
    })
    
    successAudio.value.addEventListener('error', (e) => {
      console.error('Error loading success audio:', e)
    })
  } catch (error) {
    console.error('Failed to initialize audio:', error)
  }
}

// Play success audio
const playSuccessAudio = () => {
  try {
    if (successAudio.value) {
      // Reset audio to start if it was already playing
      successAudio.value.currentTime = 0
      const playPromise = successAudio.value.play()
      
      // Handle play promise for better browser compatibility
      if (playPromise !== undefined) {
        playPromise
          .then(() => {
            console.log('Success audio played')
          })
          .catch(error => {
            console.error('Error playing success audio:', error)
          })
      }
    }
  } catch (error) {
    console.error('Failed to play success audio:', error)
  }
}

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

// Add flat properties for search/sort functionality
const usersWithFlatProperties = computed(() => {
  return props.users.map(user => ({
    ...user,
    userName: user.name, // Add flat property for search
    userProdi: user.prodi, // Add flat property for search
    status: user.rfid_code ? 'registered' : 'unregistered' // Add flat property for sort
  }))
})

// Toast Management
const addToast = (type: 'success' | 'error' | 'warning', title: string, message: string) => {
  const id = ++toastIdCounter
  const toast = { id, type, title, message }
  
  toasts.value.push(toast)
  
  // Play success audio for RFID registration success
  if (type === 'success' && title.includes('RFID Berhasil')) {
    playSuccessAudio()
  }
  
  // Auto remove after 5 seconds
  setTimeout(() => {
    removeToast(id)
  }, 5000)
}

const removeToast = (id: number) => {
  const index = toasts.value.findIndex(toast => toast.id === id)
  if (index > -1) {
    toasts.value.splice(index, 1)
  }
}

const testToast = () => {
  addToast('success', 'Test Toast', 'Toast notification bekerja dengan baik!')
}

const testAudio = () => {
  playSuccessAudio()
  addToast('success', 'Test Audio', 'Audio berhasil diputar!')
}

// DataTable columns configuration
const columns: Column[] = [
  {
    key: 'name',
    label: 'Aslab',
    sortable: true,
    width: '30%',
    searchable: true
  },
  {
    key: 'prodi',
    label: 'Program Studi',
    sortable: true,
    width: '25%',
    searchable: true
  },
  {
    key: 'semester',
    label: 'Semester',
    sortable: true,
    width: '15%',
    align: 'center'
  },
  {
    key: 'rfid_code',
    label: 'Kode RFID',
    sortable: false,
    width: '20%',
    align: 'center'
  },
  {
    key: 'status',
    label: 'Status',
    sortable: true,
    width: '10%',
    align: 'center'
  }
]

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
    onSuccess: (page) => {
      const user = props.users.find(u => u.id.toString() === form.user_id)
      addToast('success', 'RFID Berhasil Terdaftar', 
        `Kartu RFID untuk ${user?.name} telah berhasil didaftarkan dengan kode: ${form.rfid_code}`)
      
      form.reset()
      scanningStatus.value = 'idle'
      // Reload users data
      window.location.reload()
    },
    onError: (errors) => {
      if (errors.rfid_code) {
        addToast('error', 'Gagal Mendaftar RFID', errors.rfid_code)
      } else if (errors.user_id) {
        addToast('error', 'Gagal Mendaftar RFID', errors.user_id)
      } else {
        addToast('error', 'Gagal Mendaftar RFID', 'Terjadi kesalahan saat mendaftarkan RFID')
      }
    }
  })
}

const handleSearch = (query: string) => {
  // Search will be handled by DataTable internally
}

const handleSort = (column: string | null, direction: 'asc' | 'desc' | null) => {
  // Sort will be handled by DataTable internally
}

const handleRowClick = (row: User) => {
  // Could navigate to user detail or show more info
  console.log('Clicked row:', row)
}

onMounted(() => {
  // Auto start scanning if no RFID detected yet
  initializeAudio() // Initialize audio on component mount
})

onUnmounted(() => {
  stopScanning()
})
</script>
