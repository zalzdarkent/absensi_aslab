<?php

namespace App\Http\Controllers;

use App\Models\Attendance;
use App\Models\User;
use Carbon\Carbon;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Log;
use Inertia\Inertia;

class DashboardController extends Controller
{
    public function index(Request $request)
    {
        $today = Carbon::today();
        $currentMonth = $request->get('month', now()->month);
        $currentYear = $request->get('year', now()->year);

        // Statistics
        $totalAslabs = User::where('role', 'aslab')->where('is_active', true)->count();
        $todayCheckins = Attendance::where('date', $today)->where('type', 'check_in')->count();
        $todayCheckouts = Attendance::where('date', $today)->where('type', 'check_out')->count();
        $activeToday = $todayCheckins;

        // Today's attendance list
        $todayAttendances = Attendance::with('user')
            ->where('date', $today)
            ->orderBy('timestamp', 'desc')
            ->get()
            ->groupBy('user_id')
            ->map(function ($userAttendances) {
                $user = $userAttendances->first()->user;
                $checkIn = $userAttendances->where('type', 'check_in')->first();
                $checkOut = $userAttendances->where('type', 'check_out')->first();

                return [
                    'user' => [
                        'id' => $user->id,
                        'name' => $user->name,
                        'prodi' => $user->prodi,
                        'semester' => $user->semester,
                    ],
                    'check_in' => $checkIn ? $checkIn->timestamp->format('H:i') : null,
                    'check_out' => $checkOut ? $checkOut->timestamp->format('H:i') : null,
                    'status' => $checkOut ? 'Sudah Pulang' : ($checkIn ? 'Sedang di Lab' : 'Belum Datang'),
                ];
            })
            ->values();

        // Most active aslab this month
        $mostActiveAslabs = User::withCount([
            'attendances' => function ($query) use ($currentMonth, $currentYear) {
                $query->whereMonth('date', $currentMonth)
                      ->whereYear('date', $currentYear);
            }
        ])
            ->where('role', 'aslab')
            ->where('is_active', true)
            ->orderBy('attendances_count', 'desc')
            ->take(10)
            ->get()
            ->map(function ($user) {
                return [
                    'name' => $user->name,
                    'prodi' => $user->prodi,
                    'semester' => $user->semester,
                    'total_attendance' => $user->attendances_count,
                ];
            });

        // Weekly attendance chart data
        $weeklyData = [];
        for ($i = 6; $i >= 0; $i--) {
            $date = Carbon::today()->subDays($i);
            $count = Attendance::where('date', $date)
                              ->where('type', 'check_in')
                              ->count();
            $weeklyData[] = [
                'date' => $date->format('d/m'),
                'count' => $count,
            ];
        }

        return Inertia::render('Dashboard', [
            'stats' => [
                'total_aslabs' => $totalAslabs,
                'today_checkins' => $todayCheckins,
                'today_checkouts' => $todayCheckouts,
                'active_today' => $activeToday,
            ],
            'today_attendances' => $todayAttendances,
            'most_active_aslabs' => $mostActiveAslabs,
            'weekly_chart_data' => $weeklyData,
            'current_date' => $today->format('d F Y'),
        ]);
    }

    public function attendanceHistory(Request $request)
    {
        try {
            $search = $request->get('search');
            $prodi = $request->get('prodi');
            $date = $request->get('date');

            $query = Attendance::with('user')
                ->when($search, function ($q) use ($search) {
                    $q->whereHas('user', function ($userQuery) use ($search) {
                        $userQuery->where('name', 'like', "%{$search}%");
                    });
                })
                ->when($prodi, function ($q) use ($prodi) {
                    $q->whereHas('user', function ($userQuery) use ($prodi) {
                        $userQuery->where('prodi', $prodi);
                    });
                })
                ->when($date, function ($q) use ($date) {
                    $q->where('date', $date);
                })
                ->orderBy('date', 'desc')
                ->orderBy('timestamp', 'desc');

            $attendances = $query->paginate(20);

            $prodis = User::where('role', 'aslab')
                         ->where('is_active', true)
                         ->distinct()
                         ->pluck('prodi')
                         ->filter()
                         ->sort()
                         ->values();

            return Inertia::render('AttendanceHistory', [
                'attendances' => $attendances,
                'prodis' => $prodis,
                'filters' => [
                    'search' => $search,
                    'prodi' => $prodi,
                    'date' => $date,
                ],
            ]);
        } catch (\Exception $e) {
            Log::error('DashboardController@attendanceHistory error: ' . $e->getMessage());
            return back()->with('error', 'Terjadi kesalahan: ' . $e->getMessage());
        }
    }
}
