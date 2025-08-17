<?php

use App\Http\Controllers\AslabController;
use App\Http\Controllers\DashboardController;
use Illuminate\Support\Facades\Route;
use Inertia\Inertia;

Route::get('/', function () {
    return Inertia::render('Welcome');
})->name('home');

Route::middleware(['auth', 'verified'])->group(function () {
    Route::get('/dashboard', [DashboardController::class, 'index'])->name('dashboard');
    Route::get('/attendance-history', [DashboardController::class, 'attendanceHistory'])->name('attendance.history');

    // Test route untuk debug
    Route::get('/test-aslabs', function () {
        $aslabs = \App\Models\User::where('role', 'aslab')->paginate(15);
        return \Inertia\Inertia::render('Aslabs/Index', [
            'aslabs' => $aslabs,
            'prodis' => ['Teknik Informatika', 'Sistem Informasi'],
            'filters' => ['search' => null, 'prodi' => null],
        ]);
    })->name('test.aslabs');

    // Aslab management routes
    Route::resource('aslabs', AslabController::class);
    Route::patch('aslabs/{aslab}/toggle-status', [AslabController::class, 'toggleStatus'])->name('aslabs.toggle-status');
});require __DIR__.'/settings.php';
require __DIR__.'/auth.php';
