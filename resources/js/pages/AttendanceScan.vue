<template>
  <AppLayout>
    <div class="min-h-screen bg-gradient-to-br from-green-50 via-emerald-50 to-teal-50 dark:from-gray-900 dark:via-slate-900 dark:to-gray-800">
      <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
        <!-- Header Section -->
        <div class="text-center mb-8">
          <div class="inline-flex items-center justify-center w-16 h-16 bg-gradient-to-r from-green-500 to-emerald-600 dark:from-green-600 dark:to-emerald-700 rounded-full mb-4">
            <svg class="w-8 h-8 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z" />
            </svg>
          </div>
          <h1 class="text-4xl font-bold text-gray-900 dark:text-white mb-2">
            <span class="bg-gradient-to-r from-green-600 to-emerald-600 dark:from-green-400 dark:to-emerald-400 bg-clip-text text-transparent">
              Scan Absensi RFID
            </span>
          </h1>
          <p class="text-lg text-gray-600 dark:text-gray-300 max-w-2xl mx-auto">
            Scan kartu RFID untuk mencatat kehadiran asisten laboratorium
          </p>
        </div>

        <!-- Alert Messages -->
        <div v-if="lastScanResult" class="mb-8 max-w-4xl mx-auto">
          <div :class="[
            'rounded-lg p-4 shadow-sm border',
            lastScanResult.success
              ? 'bg-gradient-to-r from-green-50 to-emerald-50 dark:from-green-900 dark:to-emerald-900 border-green-200 dark:border-green-700'
              : 'bg-gradient-to-r from-red-50 to-rose-50 dark:from-red-900 dark:to-rose-900 border-red-200 dark:border-red-700'
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
                  <div class="text-sm font-medium text-gray-800 dark:text-gray-200">
                    {{ lastScanResult.data.user.name }} - {{ lastScanResult.data.user.prodi }}
                  </div>
                  <div v-if="lastScanResult.data.attendance" class="text-xs text-gray-600 dark:text-gray-400">
                    {{ lastScanResult.data.attendance.type === 'check_in' ? 'Check-in' : 'Check-out' }} pada {{ lastScanResult.data.attendance.timestamp }}
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>

        <!-- Scan Status -->
        <div class="max-w-4xl mx-auto mb-8">
          <div class="bg-white dark:bg-gray-800 rounded-2xl shadow-xl border border-gray-100 dark:border-gray-700 overflow-hidden">
            <div class="bg-gradient-to-r from-green-500 to-emerald-600 dark:from-green-600 dark:to-emerald-700 px-6 py-4">
              <div class="flex items-center justify-between">
                <div class="flex items-center">
                  <div class="flex-shrink-0">
                    <div class="w-10 h-10 bg-white bg-opacity-20 rounded-lg flex items-center justify-center">
                      <svg class="w-6 h-6 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 8v4l3 3m6-3a9 9 0 11-18 0 9 9 0 0118 0z" />
                      </svg>
                    </div>
                  </div>
                  <div class="ml-4">
                    <h3 class="text-xl font-semibold text-white">Mode Control</h3>
                    <p class="text-green-100 dark:text-green-200 text-sm">
                      Mode saat ini: <span class="font-semibold">{{ currentMode }}</span>
                    </p>
                  </div>
                </div>
                <div class="flex items-center space-x-3">
                  <!-- Mode Buttons -->
                  <button
                    @click="setMode('registration')"
                    :class="[
                      'px-4 py-2 rounded-lg font-medium text-sm transition-all duration-200',
                      currentMode === 'registration'
                        ? 'bg-white text-green-600 shadow-md'
                        : 'bg-green-600 hover:bg-green-500 text-white'
                    ]"
                  >
                    Registration
                  </button>
                  <button
                    @click="setMode('check_in')"
                    :class="[
                      'px-4 py-2 rounded-lg font-medium text-sm transition-all duration-200',
                      currentMode === 'check_in'
                        ? 'bg-white text-green-600 shadow-md'
                        : 'bg-green-600 hover:bg-green-500 text-white'
                    ]"
                  >
                    Check In
                  </button>
                  <button
                    @click="setMode('check_out')"
                    :class="[
                      'px-4 py-2 rounded-lg font-medium text-sm transition-all duration-200',
                      currentMode === 'check_out'
                        ? 'bg-white text-green-600 shadow-md'
                        : 'bg-green-600 hover:bg-green-500 text-white'
                    ]"
                  >
                    Check Out
                  </button>
                </div>
              </div>
            </div>
          </div>
        </div>

        <!-- Stats and Recent Scans -->
        <div class="max-w-4xl mx-auto">
          <div class="bg-white dark:bg-gray-800 rounded-2xl shadow-xl border border-gray-100 dark:border-gray-700 overflow-hidden">
            <div class="bg-gradient-to-r from-blue-500 to-indigo-600 dark:from-blue-600 dark:to-indigo-700 px-6 py-4">
              <div class="flex items-center justify-between">
                <div class="flex items-center">
                  <div class="flex-shrink-0">
                    <div class="w-10 h-10 bg-white bg-opacity-20 rounded-lg flex items-center justify-center">
                      <svg class="w-6 h-6 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 19v-6a2 2 0 00-2-2H5a2 2 0 00-2 2v6a2 2 0 002 2h2a2 2 0 002-2zm0 0V9a2 2 0 012-2h2a2 2 0 012 2v10m-6 0a2 2 0 002 2h2a2 2 0 002-2m0 0V5a2 2 0 012-2h2a2 2 0 012 2v14a2 2 0 01-2 2h-2a2 2 0 01-2-2z" />
                      </svg>
                    </div>
                  </div>
                  <div class="ml-4">
                    <h3 class="text-xl font-semibold text-white">Statistik Hari Ini</h3>
                    <p class="text-blue-100 dark:text-blue-200 text-sm">
                      {{ scanningActive ? 'Monitoring aktif' : 'Monitoring tidak aktif' }}
                    </p>
                  </div>
                </div>
                <div class="flex items-center space-x-4">
                  <div :class="[
                    'px-4 py-2 rounded-lg font-medium text-sm',
                    scanningActive
                      ? 'bg-green-500 dark:bg-green-600 text-white'
                      : 'bg-gray-500 dark:bg-gray-600 text-white'
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
                </div>
              </div>
            </div>

            <div class="p-6 dark:bg-gray-800">
              <div class="grid grid-cols-1 md:grid-cols-3 gap-6">
                <!-- Scan Count Today -->
                <div class="bg-gradient-to-br from-blue-50 to-indigo-50 dark:from-blue-900 dark:to-indigo-900 rounded-xl p-4 border border-blue-200 dark:border-blue-700">
                  <div class="flex items-center">
                    <div class="w-10 h-10 bg-blue-500 dark:bg-blue-600 rounded-lg flex items-center justify-center">
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
                <div class="bg-gradient-to-br from-orange-50 to-red-50 dark:from-orange-900 dark:to-red-900 rounded-xl p-4 border border-orange-200 dark:border-orange-700">
                  <div class="flex items-center">
                    <div class="w-10 h-10 bg-orange-500 dark:bg-orange-600 rounded-lg flex items-center justify-center">
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
                <div class="bg-gradient-to-br from-green-50 to-emerald-50 dark:from-green-900 dark:to-emerald-900 rounded-xl p-4 border border-green-200 dark:border-green-700">
                  <div class="flex items-center">
                    <div class="w-10 h-10 bg-green-500 dark:bg-green-600 rounded-lg flex items-center justify-center">
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
              <div class="mt-6">
                <h4 class="text-lg font-semibold text-gray-900 dark:text-white mb-4">Scan Terbaru</h4>
                <div class="space-y-2">
                  <div v-if="recentScans.length === 0" class="text-center py-8 text-gray-500 dark:text-gray-400">
                    Belum ada scan hari ini
                  </div>
                  <div v-else v-for="scan in recentScans" :key="scan.id"
                    class="flex items-center justify-between p-3 bg-gray-50 dark:bg-gray-700 rounded-lg">
                    <div class="flex items-center">
                      <div :class="[
                        'w-3 h-3 rounded-full mr-3',
                        scan.type === 'check_in' ? 'bg-green-500' : 'bg-orange-500'
                      ]"></div>
                      <div>
                        <p class="font-medium text-gray-900 dark:text-white">{{ scan.user.name }}</p>
                        <p class="text-xs text-gray-500 dark:text-gray-400">{{ scan.user.prodi }}</p>
                      </div>
                    </div>
                    <div class="text-right">
                      <p class="text-sm font-medium text-gray-900 dark:text-white">
                        {{ scan.type === 'check_in' ? 'Check-in' : 'Check-out' }}
                      </p>
                      <p class="text-xs text-gray-500 dark:text-gray-400">{{ formatTime(scan.timestamp) }}</p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </AppLayout>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'
import AppLayout from '@/layouts/AppLayout.vue'

interface User {
  id: number
  name: string
  prodi: string
  semester: number
}

interface AttendanceRecord {
  id: string | number
  user: User
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

const scanningActive = ref(false)
const currentMode = ref('registration')
const lastScanResult = ref<ScanResult | null>(null)
const recentScans = ref<AttendanceRecord[]>([])
const todayStats = ref({
  checkInCount: 0,
  checkOutCount: 0,
  activeUsers: 0
})

let scanInterval: number | null = null

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
      }
    }
  } catch (error) {
    console.error('Error setting mode:', error)
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
  } else {
    stopScanning()
  }
}

const startScanning = () => {
  console.log('Started attendance scanning...')

  // Poll server setiap 2 detik untuk cek scan baru
  scanInterval = setInterval(async () => {
    try {
      // Cek apakah ada data baru dari Arduino
      const response = await fetch('/api/rfid/attendance-today')
      if (response.ok) {
        const data = await response.json()
        if (data.success) {
          updateTodayStats(data.data)
          updateRecentScans(data.data.attendances)
        }
      }
    } catch (error) {
      console.error('Scanning error:', error)
    }
  }, 2000)
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
          type: 'check_in' as const,
          timestamp: att.check_in,
          date: new Date().toISOString().split('T')[0]
        })
      }
      if (att.check_out) {
        scans.push({
          id: `${att.user.id}-out`,
          user: att.user,
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

const formatTime = (timeString: string) => {
  return timeString // Already in HH:mm:ss format
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

  // Auto start scanning
  scanningActive.value = true
  startScanning()
})

onUnmounted(() => {
  stopScanning()
})
</script>
