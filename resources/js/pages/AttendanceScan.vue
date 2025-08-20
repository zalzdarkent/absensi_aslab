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
  </AppLayout>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted, computed } from 'vue'
import AppLayout from '@/layouts/AppLayout.vue'
import DataTable, { type Column } from '@/components/DataTable.vue'
import { History, Clock, CheckCircle, XCircle } from 'lucide-vue-next'

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
