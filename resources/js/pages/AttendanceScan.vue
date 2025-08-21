<template>
  <AppLayout>
    <div class="flex h-full flex-1 flex-col gap-6 rounded-xl p-6">
      <!-- Header -->
      <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between">
        <div>
          <h1 class="text-2xl font-bold text-foreground">Scan Absensi RFID</h1>
          <p class="mt-1 text-sm text-muted-foreground">Scan kartu RFID untuk mencatat kehadiran asisten laboratorium</p>
        </div>
      </div>

      <!-- Alert Messages -->
      <div v-if="lastScanResult" class="mb-6">
        <div :class="[
          'rounded-lg p-4 border',
          lastScanResult.success
            ? 'bg-green-50 dark:bg-green-900/20 border-green-200 dark:border-green-700'
            : 'bg-red-50 dark:bg-red-900/20 border-red-200 dark:border-red-700'
        ]">
          <div class="flex">
            <div class="flex-shrink-0">
              <div :class="[
                'w-8 h-8 rounded-full flex items-center justify-center',
                lastScanResult.success
                  ? 'bg-green-100 dark:bg-green-800'
                  : 'bg-red-100 dark:bg-red-800'
              ]">
                <svg v-if="lastScanResult.success" class="h-5 w-5 text-green-600 dark:text-green-300" viewBox="0 0 20 20" fill="currentColor">
                  <path fill-rule="evenodd" d="M10 18a8 8 0 100-16 8 8 0 000 16zm3.857-9.809a.75.75 0 00-1.214-.882l-3.236 4.53L7.53 10.23a.75.75 0 00-1.06 1.061l2.5 2.5a.75.75 0 001.137-.089l4-5.5z" clip-rule="evenodd" />
                </svg>
                <svg v-else class="h-5 w-5 text-red-600 dark:text-red-300" viewBox="0 0 20 20" fill="currentColor">
                  <path fill-rule="evenodd" d="M10 18a8 8 0 100-16 8 8 0 000 16zM8.28 7.22a.75.75 0 00-1.06 1.06L8.94 10l-1.72 1.72a.75.75 0 101.06 1.06L10 11.06l1.72 1.72a.75.75 0 101.06-1.06L11.06 10l1.72-1.72a.75.75 0 00-1.06-1.06L10 8.94 8.28 7.22z" clip-rule="evenodd" />
                </svg>
              </div>
            </div>
            <div class="ml-3">
              <h3 :class="[
                'text-sm font-medium',
                lastScanResult.success
                  ? 'text-green-800 dark:text-green-200'
                  : 'text-red-800 dark:text-red-200'
              ]">
                {{ lastScanResult.success ? 'Berhasil!' : 'Gagal!' }}
              </h3>
              <p :class="[
                'text-sm mt-1',
                lastScanResult.success
                  ? 'text-green-700 dark:text-green-300'
                  : 'text-red-700 dark:text-red-300'
              ]">
                {{ lastScanResult.message }}
              </p>
              <div v-if="lastScanResult.data && lastScanResult.data.user" class="mt-2">
                <div class="text-sm font-medium text-foreground">
                  {{ lastScanResult.data.user.name }} - {{ lastScanResult.data.user.prodi }}
                </div>
                <div v-if="lastScanResult.data.attendance" class="text-xs text-muted-foreground">
                  {{ lastScanResult.data.attendance.type === 'check_in' ? 'Check-in' : 'Check-out' }} pada {{ lastScanResult.data.attendance.timestamp }}
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- Mode Control -->
      <div class="bg-card rounded-lg border border-border p-6">
        <div class="flex items-center justify-between mb-4">
          <div class="flex items-center">
            <div class="w-10 h-10 bg-green-500 rounded-lg flex items-center justify-center mr-4">
              <svg class="w-6 h-6 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 8v4l3 3m6-3a9 9 0 11-18 0 9 9 0 0118 0z" />
              </svg>
            </div>
            <div>
              <h3 class="text-xl font-semibold text-foreground">Mode Control</h3>
              <p class="text-muted-foreground text-sm">
                Mode saat ini:
                <span class="font-semibold text-foreground">
                  <template v-if="currentMode === 'registration'" >Registrasi</template>
                  <template v-else-if="currentMode === 'check_in'" >Check In</template>
                  <template v-else-if="currentMode === 'check_out'" >Check Out</template>
                  <template v-else>{{ currentMode }}</template>
                </span>
              </p>
            </div>
          </div>
          <div class="flex items-center space-x-3">
            <button
              @click="setMode('registration')"
              :class="[
                'px-4 py-2 rounded-lg font-medium text-sm transition-all duration-200 cursor-pointer',
                currentMode === 'registration'
                  ? 'bg-primary text-primary-foreground'
                  : 'bg-secondary text-secondary-foreground hover:bg-secondary/80'
              ]"
            >
              Registration
            </button>
            <button
              @click="setMode('check_in')"
              :class="[
                'px-4 py-2 rounded-lg font-medium text-sm transition-all duration-200 cursor-pointer',
                currentMode === 'check_in'
                  ? 'bg-primary text-primary-foreground'
                  : 'bg-secondary text-secondary-foreground hover:bg-secondary/80'
              ]"
            >
              Check In
            </button>
            <button
              @click="setMode('check_out')"
              :class="[
                'px-4 py-2 rounded-lg font-medium text-sm transition-all duration-200 cursor-pointer',
                currentMode === 'check_out'
                  ? 'bg-primary text-primary-foreground'
                  : 'bg-secondary text-secondary-foreground hover:bg-secondary/80'
              ]"
            >
              Check Out
            </button>
          </div>
        </div>
      </div>

      <!-- Stats and Recent Scans -->
      <div class="bg-card rounded-lg border border-border p-6">
        <div class="flex items-center justify-between mb-6">
          <div class="flex items-center">
            <div class="w-10 h-10 bg-blue-500 rounded-lg flex items-center justify-center mr-4">
              <svg class="w-6 h-6 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 19v-6a2 2 0 00-2-2H5a2 2 0 00-2 2v6a2 2 0 002 2h2a2 2 0 002-2zm0 0V9a2 2 0 012-2h2a2 2 0 012 2v10m-6 0a2 2 0 002 2h2a2 2 0 002-2m0 0V5a2 2 0 012-2h2a2 2 0 012 2v14a2 2 0 01-2 2h-2a2 2 0 01-2-2z" />
              </svg>
            </div>
            <div>
              <h3 class="text-xl font-semibold text-foreground">Statistik Hari Ini</h3>
              <p class="text-muted-foreground text-sm">
                {{ scanningActive ? 'Monitoring aktif' : 'Monitoring tidak aktif' }}
              </p>
            </div>
          </div>
          <div class="flex items-center space-x-4">
            <div :class="[
              'px-4 py-2 rounded-lg font-medium text-sm',
              scanningActive
                ? 'bg-green-500 text-white'
                : 'bg-gray-500 text-white'
            ]">
              {{ scanningActive ? 'AKTIF' : 'NONAKTIF' }}
            </div>
            <button
              @click="toggleScanning"
              :class="[
                'px-4 py-2 rounded-lg font-medium text-sm transition-all duration-200',
                scanningActive
                  ? 'bg-red-500 hover:bg-red-600 text-white'
                  : 'bg-blue-500 hover:bg-blue-600 text-white'
              ]"
            >
              {{ scanningActive ? 'Stop' : 'Start' }}
            </button>
            <!-- Test Toast Button (for debugging) -->
            <button
              @click="testToast"
              class="px-4 py-2 rounded-lg font-medium text-sm bg-purple-500 hover:bg-purple-600 text-white transition-all duration-200"
            >
              Test Toast
            </button>
            <!-- Test Audio Button (for debugging) -->
            <button
              @click="testAudio"
              class="px-4 py-2 rounded-lg font-medium text-sm bg-orange-500 hover:bg-orange-600 text-white transition-all duration-200"
            >
              Test Audio
            </button>
          </div>
        </div>

        <!-- Stats Cards -->
        <div class="grid grid-cols-1 md:grid-cols-3 gap-4 mb-6">
          <!-- Check-in Count -->
          <div class="bg-blue-50 dark:bg-blue-900/20 rounded-lg p-4 border border-blue-200 dark:border-blue-700">
            <div class="flex items-center">
              <div class="w-10 h-10 bg-blue-500 rounded-lg flex items-center justify-center">
                <svg class="w-5 h-5 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z" />
                </svg>
              </div>
              <div class="ml-3">
                <p class="text-sm font-medium text-blue-800 dark:text-blue-200">Check-in Hari Ini</p>
                <p class="text-2xl font-bold text-blue-900 dark:text-blue-100">{{ todayStats.checkInCount }}</p>
              </div>
            </div>
          </div>

          <!-- Check-out Count -->
          <div class="bg-orange-50 dark:bg-orange-900/20 rounded-lg p-4 border border-orange-200 dark:border-orange-700">
            <div class="flex items-center">
              <div class="w-10 h-10 bg-orange-500 rounded-lg flex items-center justify-center">
                <svg class="w-5 h-5 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M17 16l4-4m0 0l-4-4m4 4H7m6 4v1a3 3 0 01-3 3H6a3 3 0 01-3-3V7a3 3 0 013-3h4a3 3 0 013 3v1" />
                </svg>
              </div>
              <div class="ml-3">
                <p class="text-sm font-medium text-orange-800 dark:text-orange-200">Check-out Hari Ini</p>
                <p class="text-2xl font-bold text-orange-900 dark:text-orange-100">{{ todayStats.checkOutCount }}</p>
              </div>
            </div>
          </div>

          <!-- Active Users -->
          <div class="bg-green-50 dark:bg-green-900/20 rounded-lg p-4 border border-green-200 dark:border-green-700">
            <div class="flex items-center">
              <div class="w-10 h-10 bg-green-500 rounded-lg flex items-center justify-center">
                <svg class="w-5 h-5 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 4.354a4 4 0 110 5.292M15 21H3v-1a6 6 0 0112 0v1zm0 0h6v-1a6 6 0 00-9-5.197m13.5-9a2.5 2.5 0 11-5 0 2.5 2.5 0 015 0z" />
                </svg>
              </div>
              <div class="ml-3">
                <p class="text-sm font-medium text-green-800 dark:text-green-200">Total Aktif</p>
                <p class="text-2xl font-bold text-green-900 dark:text-green-100">{{ todayStats.activeUsers }}</p>
              </div>
            </div>
          </div>
        </div>

        <!-- Recent Scans -->
        <div>
          <h4 class="text-lg font-semibold text-foreground mb-4">Scan Terbaru</h4>
          
          <!-- DataTable for Recent Scans -->
          <DataTable
            :columns="columns"
            :data="recentScans"
            :empty-message="'Belum ada scan hari ini'"
            :empty-icon="History"
            :show-refresh="false"
            :show-export="false"
            :pagination="undefined"
            @search="handleSearch"
            @sort="handleSort"
            @row-click="handleRowClick"
          >
            <!-- Custom cell templates -->
            <template #cell-userName="{ row }">
              <div>
                <div class="text-sm font-medium text-foreground">{{ row.user.name }}</div>
                <div class="text-sm text-muted-foreground">{{ row.user.prodi }}</div>
              </div>
            </template>

            <template #cell-type="{ row }">
              <span :class="[
                'inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium',
                getTypeColor(row.type)
              ]">
                <component :is="getTypeIcon(row.type)" class="h-3 w-3 mr-1" />
                {{ getTypeText(row.type) }}
              </span>
            </template>

            <template #cell-timestamp="{ value }">
              <span class="text-sm text-foreground font-mono">{{ formatTime(value) }}</span>
            </template>

            <template #cell-date="{ value }">
              <span class="text-sm text-foreground">{{ formatDate(value) }}</span>
            </template>
          </DataTable>
        </div>
      </div>
    </div>

    <!-- Toast Notifications -->
    <div class="fixed top-4 right-4 z-50 space-y-3">
      <div
        v-for="toast in toasts"
        :key="toast.id"
        :class="[
          'max-w-sm w-full rounded-lg shadow-lg border transform transition-all duration-300 ease-in-out',
          toast.type === 'success' 
            ? 'bg-green-50 dark:bg-green-900/20 border-green-200 dark:border-green-700'
            : toast.type === 'error'
            ? 'bg-red-50 dark:bg-red-900/20 border-red-200 dark:border-red-700' 
            : 'bg-blue-50 dark:bg-blue-900/20 border-blue-200 dark:border-blue-700'
        ]"
      >
        <div class="p-4">
          <div class="flex items-start">
            <div class="flex-shrink-0">
              <div :class="[
                'w-8 h-8 rounded-full flex items-center justify-center',
                toast.type === 'success'
                  ? 'bg-green-100 dark:bg-green-800'
                  : toast.type === 'error'
                  ? 'bg-red-100 dark:bg-red-800'
                  : 'bg-blue-100 dark:bg-blue-800'
              ]">
                <CheckCircle v-if="toast.type === 'success'" class="h-5 w-5 text-green-600 dark:text-green-300" />
                <XCircle v-else-if="toast.type === 'error'" class="h-5 w-5 text-red-600 dark:text-red-300" />
                <Clock v-else class="h-5 w-5 text-blue-600 dark:text-blue-300" />
              </div>
            </div>
            <div class="ml-3 flex-1">
              <h3 :class="[
                'text-sm font-medium',
                toast.type === 'success'
                  ? 'text-green-800 dark:text-green-200'
                  : toast.type === 'error'
                  ? 'text-red-800 dark:text-red-200'
                  : 'text-blue-800 dark:text-blue-200'
              ]">
                {{ toast.title }}
              </h3>
              <p :class="[
                'text-sm mt-1',
                toast.type === 'success'
                  ? 'text-green-700 dark:text-green-300'
                  : toast.type === 'error'
                  ? 'text-red-700 dark:text-red-300'
                  : 'text-blue-700 dark:text-blue-300'
              ]">
                {{ toast.message }}
              </p>
              <div v-if="toast.data && toast.data.user" class="mt-2">
                <div class="text-sm font-medium text-foreground">
                  {{ toast.data.user.name }} - {{ toast.data.user.prodi }}
                </div>
                <div v-if="toast.data.attendance" class="text-xs text-muted-foreground">
                  {{ getTypeText(toast.data.attendance.type) }} pada {{ toast.data.attendance.timestamp }}
                </div>
              </div>
              <div class="text-xs text-muted-foreground mt-1">
                {{ formatToastTime(toast.timestamp) }}
              </div>
            </div>
            <div class="ml-4 flex-shrink-0">
              <button
                @click="removeToast(toast.id)"
                class="rounded-md p-1.5 hover:bg-gray-100 dark:hover:bg-gray-700 transition-colors"
              >
                <X class="h-4 w-4 text-gray-400" />
              </button>
            </div>
          </div>
        </div>
      </div>
    </div>
  </AppLayout>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted, computed } from 'vue'
import AppLayout from '@/layouts/AppLayout.vue'
import DataTable, { type Column } from '@/components/DataTable.vue'
import { History, Clock, CheckCircle, XCircle, X } from 'lucide-vue-next'

interface User {
  id: number
  name: string
  prodi: string
  semester: number
}

interface AttendanceRecord {
  id: string | number
  user: User
  userName: string // Add flat property for search/sort
  userProdi: string // Add flat property for search/sort
  type: 'check_in' | 'check_out'
  timestamp: string
  date: string
}

interface ScanResult {
  success: boolean
  message: string
  data?: {
    user: User
    attendance: {
      type: 'check_in' | 'check_out'
      timestamp: string
    }
  }
}

interface ToastNotification {
  id: string
  type: 'success' | 'error' | 'info'
  title: string
  message: string
  data?: any
  timestamp: Date
}

interface RecentScan {
  user: User
  type: 'check_in' | 'check_out'
  timestamp: string
  scanKey: string
}

const scanningActive = ref(false)
const currentMode = ref('registration')
const lastScanResult = ref<ScanResult | null>(null)
const recentScans = ref<AttendanceRecord[]>([])
const todayStats = ref({
  checkInCount: 0,
  checkOutCount: 0,
  activeUsers: 0
})
const toasts = ref<ToastNotification[]>([])
const lastProcessedScan = ref<string>('')
const lastProcessedTimestamp = ref<string>('') // Track last processed timestamp

let scanInterval: number | null = null

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

// DataTable columns configuration
const columns: Column[] = [
  {
    key: 'userName',
    label: 'Aslab',
    sortable: true,
    width: '30%',
    searchable: true
  },
  {
    key: 'type',
    label: 'Tipe',
    sortable: true,
    width: '20%',
    align: 'center'
  },
  {
    key: 'timestamp',
    label: 'Waktu',
    sortable: true,
    width: '25%',
    align: 'center'
  },
  {
    key: 'date',
    label: 'Tanggal',
    sortable: true,
    width: '25%',
    align: 'center'
  }
]

// Toast notification functions
const addToast = (type: 'success' | 'error' | 'info', title: string, message: string, data?: any) => {
  // Create simple throttle key to prevent only immediate duplicates
  const throttleKey = `${type}-${title}-${Date.now()}`
  
  const id = Date.now().toString() + Math.random().toString(36).substr(2, 9)
  const toast: ToastNotification = {
    id,
    type,
    title,
    message,
    data,
    timestamp: new Date()
  }
  
  toasts.value.unshift(toast)
  
  // Play success audio for check-in/check-out success
  if (type === 'success' && (title.includes('Check-in') || title.includes('Check-out'))) {
    playSuccessAudio()
  }
  
  // Limit to maximum 5 toasts
  if (toasts.value.length > 5) {
    toasts.value = toasts.value.slice(0, 5)
  }
  
  // Auto remove after 5 seconds for success/info, 8 seconds for error
  const timeout = type === 'error' ? 8000 : 5000
  setTimeout(() => {
    removeToast(id)
  }, timeout)
}

const removeToast = (id: string) => {
  const index = toasts.value.findIndex(toast => toast.id === id)
  if (index > -1) {
    toasts.value.splice(index, 1)
  }
}

const formatToastTime = (timestamp: Date) => {
  return timestamp.toLocaleTimeString('id-ID', {
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit'
  })
}

const setMode = async (mode: 'registration' | 'check_in' | 'check_out') => {
  try {
    const response = await fetch('/api/rfid/set-mode', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'X-CSRF-TOKEN': document.querySelector('meta[name="csrf-token"]')?.getAttribute('content') || ''
      },
      body: JSON.stringify({ mode })
    })

    if (response.ok) {
      const data = await response.json()
      if (data.success) {
        currentMode.value = mode
        console.log(`Mode changed to: ${mode}`)
        
        // Show toast notification for mode change
        const modeText = mode === 'registration' ? 'Registrasi' : mode === 'check_in' ? 'Check In' : 'Check Out'
        addToast('info', 'Mode Berubah', `Mode sistem diubah ke: ${modeText}`)
      }
    } else {
      const errorData = await response.json()
      addToast('error', 'Gagal Ubah Mode', errorData.message || 'Terjadi kesalahan saat mengubah mode')
    }
  } catch (error) {
    console.error('Error setting mode:', error)
    addToast('error', 'Koneksi Error', 'Tidak dapat terhubung ke server')
  }
}

const fetchCurrentMode = async () => {
  try {
    const response = await fetch('/api/rfid/get-mode-command')
    if (response.ok) {
      const data = await response.json()
      currentMode.value = data.mode
    }
  } catch (error) {
    console.error('Error fetching current mode:', error)
  }
}

const toggleScanning = () => {
  scanningActive.value = !scanningActive.value

  if (scanningActive.value) {
    startScanning()
    addToast('info', 'Monitoring Aktif', 'Sistem monitoring RFID telah diaktifkan')
  } else {
    stopScanning()
    addToast('info', 'Monitoring Berhenti', 'Sistem monitoring RFID telah dihentikan')
  }
}

const startScanning = () => {
  console.log('Started attendance scanning...')

  // Initialize to avoid showing old scans as new
  lastProcessedTimestamp.value = new Date().toISOString()

  // Poll server setiap 2 detik untuk cek scan baru
  scanInterval = setInterval(async () => {
    try {
      // Primary method: Check attendance-today for stats
      const todayResponse = await fetch('/api/rfid/attendance-today')
      if (todayResponse.ok) {
        const todayData = await todayResponse.json()
        if (todayData.success) {
          updateTodayStats(todayData.data)
          updateRecentScans(todayData.data.attendances)
          checkForNewScans(todayData.data.attendances)
        }
      }

      // Secondary method: Check latest attendance logs for real-time detection
      const logsResponse = await fetch('/api/rfid/attendance-logs?per_page=3')
      if (logsResponse.ok) {
        const logsData = await logsResponse.json()
        if (logsData.success && logsData.data.attendances.length > 0) {
          checkForRealTimeScans(logsData.data.attendances)
        }
      }
    } catch (error) {
      console.error('Scanning error:', error)
      // Show error toast occasionally
      if (Math.random() < 0.05) { // 5% chance to show error
        handleScanError(error)
      }
    }
  }, 2000) // Back to 2 seconds for better responsiveness
}

// Check for real-time scans from attendance logs
const checkForRealTimeScans = (logs: any[]) => {
  if (!logs || logs.length === 0) return
  
  // Get the most recent log entry
  const latestLog = logs[0]
  const scanKey = `log-${latestLog.id}-${latestLog.user_id}-${latestLog.type}-${latestLog.timestamp}`
  
  // Check if this is a new scan we haven't processed
  if (scanKey !== lastProcessedScan.value) {
    // Check if the scan is recent (within last 30 seconds)
    const scanTime = new Date(latestLog.timestamp)
    const now = new Date()
    const timeDifference = (now.getTime() - scanTime.getTime()) / 1000
    
    console.log('Checking real-time scan:', scanKey, 'Time diff:', timeDifference, 'seconds')
    
    // Only show toast for very recent scans
    if (timeDifference >= 0 && timeDifference <= 30) {
      lastProcessedScan.value = scanKey
      
      const title = latestLog.type === 'check_in' ? 'Check-in Berhasil!' : 'Check-out Berhasil!'
      const message = latestLog.type === 'check_in' 
        ? `Selamat datang, ${latestLog.user.name}!`
        : `Sampai jumpa, ${latestLog.user.name}!`
      
      console.log('Showing real-time toast for:', scanKey)
      
      addToast('success', title, message, {
        user: latestLog.user,
        attendance: {
          type: latestLog.type,
          timestamp: latestLog.timestamp
        }
      })
    }
  }
}

const checkForNewScans = (attendances: any[]) => {
  // Skip if no attendances
  if (!attendances || attendances.length === 0) return
  
  // Get all scans from today and sort by timestamp
  const allScans: any[] = []
  
  attendances.forEach(att => {
    if (att.check_in) {
      allScans.push({
        user: att.user,
        type: 'check_in',
        timestamp: att.check_in,
        scanKey: `${att.user.id}-check_in-${att.check_in}`,
        fullTimestamp: new Date(`${new Date().toDateString()} ${att.check_in}`)
      })
    }
    if (att.check_out) {
      allScans.push({
        user: att.user,
        type: 'check_out',
        timestamp: att.check_out,
        scanKey: `${att.user.id}-check_out-${att.check_out}`,
        fullTimestamp: new Date(`${new Date().toDateString()} ${att.check_out}`)
      })
    }
  })
  
  // Sort by full timestamp (newest first)
  allScans.sort((a, b) => b.fullTimestamp.getTime() - a.fullTimestamp.getTime())
  
  if (allScans.length > 0) {
    const latestScan = allScans[0]
    
    // Check if this is a new scan we haven't processed yet
    if (latestScan.scanKey !== lastProcessedScan.value) {
      // Check if scan is recent (within last 2 minutes to be safe)
      const now = new Date()
      const scanTime = latestScan.fullTimestamp
      const timeDifference = (now.getTime() - scanTime.getTime()) / 1000 // in seconds
      
      console.log('Checking scan:', latestScan.scanKey, 'Time diff:', timeDifference, 'seconds')
      
      // Only show toast for recent scans (within 2 minutes = 120 seconds)
      if (timeDifference >= 0 && timeDifference <= 120) {
        lastProcessedScan.value = latestScan.scanKey
        
        // Create a toast notification for the new scan
        const title = latestScan.type === 'check_in' ? 'Check-in Berhasil!' : 'Check-out Berhasil!'
        const message = latestScan.type === 'check_in' 
          ? `Selamat datang, ${latestScan.user.name}!`
          : `Sampai jumpa, ${latestScan.user.name}!`
        
        console.log('Showing toast for new scan:', latestScan.scanKey)
        
        addToast('success', title, message, {
          user: latestScan.user,
          attendance: {
            type: latestScan.type,
            timestamp: latestScan.timestamp
          }
        })
      } else {
        console.log('Scan too old or in future, not showing toast')
      }
    }
  }
}

const checkForDirectScans = async () => {
  // This function checks for the most recent scan from the attendance logs
  try {
    const scanResponse = await fetch('/api/rfid/attendance-logs?per_page=1')
    if (scanResponse.ok) {
      const scanData = await scanResponse.json()
      if (scanData.success && scanData.data.attendances.length > 0) {
        const latestScan = scanData.data.attendances[0]
        
        // Create unique scan key using actual timestamp
        const scanKey = `${latestScan.user_id}-${latestScan.type}-${latestScan.timestamp}`
        
        // Check if this is a new scan (within last 15 seconds) and not already processed
        const scanTime = new Date(latestScan.timestamp)
        const now = new Date()
        const timeDiff = (now.getTime() - scanTime.getTime()) / 1000
        
        // Only show toast if scan is recent and hasn't been processed
        if (timeDiff <= 15 && scanKey !== lastProcessedScan.value) {
          lastProcessedScan.value = scanKey
          
          // Show toast for real-time scan
          const title = latestScan.type === 'check_in' ? 'Check-in Berhasil!' : 'Check-out Berhasil!'
          const message = latestScan.type === 'check_in' 
            ? `Selamat datang, ${latestScan.user.name}!`
            : `Sampai jumpa, ${latestScan.user.name}!`
          
          addToast('success', title, message, {
            user: latestScan.user,
            attendance: {
              type: latestScan.type,
              timestamp: latestScan.timestamp
            }
          })
        }
      }
    }
  } catch (error) {
    console.error('Error checking for direct scans:', error)
  }
}

const stopScanning = () => {
  if (scanInterval) {
    clearInterval(scanInterval)
    scanInterval = null
  }
  console.log('Stopped attendance scanning')
}

const updateTodayStats = (data: any) => {
  todayStats.value = {
    checkInCount: data.summary.total_check_in,
    checkOutCount: data.summary.total_check_out,
    activeUsers: data.summary.active_users
  }
}

const updateRecentScans = (attendances: any[]) => {
  // Convert to our format and take last 10
  const formatted = attendances
    .filter(att => att.check_in || att.check_out)
    .flatMap(att => {
      const scans = []
      if (att.check_in) {
        scans.push({
          id: `${att.user.id}-in`,
          user: att.user,
          userName: att.user.name, // Add flat property for search/sort
          userProdi: att.user.prodi, // Add flat property for search/sort
          type: 'check_in' as const,
          timestamp: att.check_in,
          date: new Date().toISOString().split('T')[0]
        })
      }
      if (att.check_out) {
        scans.push({
          id: `${att.user.id}-out`,
          user: att.user,
          userName: att.user.name, // Add flat property for search/sort
          userProdi: att.user.prodi, // Add flat property for search/sort
          type: 'check_out' as const,
          timestamp: att.check_out,
          date: new Date().toISOString().split('T')[0]
        })
      }
      return scans
    })
    .sort((a, b) => b.timestamp.localeCompare(a.timestamp))
    .slice(0, 10)

  recentScans.value = formatted
}

// Function to simulate or handle RFID scan errors
const handleScanError = (error: any) => {
  let errorMessage = 'Terjadi kesalahan saat scanning RFID'
  let errorTitle = 'Scan Gagal'
  
  if (error.message) {
    errorMessage = error.message
  }
  
  if (error.response) {
    // If it's a response from the API
    try {
      const errorData = JSON.parse(error.response)
      if (errorData.message) {
        errorMessage = errorData.message
      }
      if (errorData.success === false) {
        if (errorMessage.includes('tidak terdaftar')) {
          errorTitle = 'RFID Tidak Terdaftar'
        } else if (errorMessage.includes('sudah')) {
          errorTitle = 'Sudah Absen'
        }
      }
    } catch (e) {
      // Ignore JSON parse error
    }
  }
  
  addToast('error', errorTitle, errorMessage)
}

const formatTime = (timeString: string) => {
  return timeString // Already in HH:mm:ss format
}

const formatDate = (dateString: string) => {
  return new Date(dateString).toLocaleDateString('id-ID', {
    weekday: 'short',
    day: 'numeric',
    month: 'short',
    year: 'numeric'
  })
}

const getTypeIcon = (type: string) => {
  return type === 'check_in' ? CheckCircle : XCircle
}

const getTypeColor = (type: string) => {
  return type === 'check_in' 
    ? 'bg-green-100 text-green-800 dark:bg-green-900/20 dark:text-green-400'
    : 'bg-orange-100 text-orange-800 dark:bg-orange-900/20 dark:text-orange-400'
}

const getTypeText = (type: string) => {
  return type === 'check_in' ? 'Check-in' : 'Check-out'
}

const handleSearch = (query: string) => {
  // Filter recentScans based on search query
  // This will be handled by DataTable internally for client-side filtering
}

const handleSort = (column: string | null, direction: 'asc' | 'desc' | null) => {
  // Sort recentScans based on column and direction
  // This will be handled by DataTable internally for client-side sorting
}

const handleRowClick = (row: AttendanceRecord) => {
  // Could navigate to user detail or show more info
  console.log('Clicked row:', row)
}

// Test function to verify toast is working
const testToast = () => {
  addToast('success', 'Test Toast', 'Sistem toast notification berfungsi dengan baik!', {
    user: { name: 'Test User', prodi: 'Test Prodi' },
    attendance: { type: 'check_in', timestamp: '10:30:00' }
  })
}

// Test function to verify audio is working
const testAudio = () => {
  playSuccessAudio()
  addToast('info', 'Test Audio', 'Audio berhasil diputar!')
}

const loadInitialData = async () => {
  try {
    const response = await fetch('/api/rfid/attendance-today')
    if (response.ok) {
      const data = await response.json()
      if (data.success) {
        updateTodayStats(data.data)
        updateRecentScans(data.data.attendances)
      }
    }
  } catch (error) {
    console.error('Error loading initial data:', error)
  }
}

onMounted(() => {
  loadInitialData()
  fetchCurrentMode()
  initializeAudio() // Initialize audio on component mount

  // Auto start scanning
  scanningActive.value = true
  startScanning()
})

onUnmounted(() => {
  stopScanning()
})
</script>
