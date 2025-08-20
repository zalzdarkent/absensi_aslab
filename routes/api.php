<?php

use App\Http\Controllers\Api\RfidController;
use App\Http\Controllers\Api\RfidRegistrationController;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;

Route::get('/user', function (Request $request) {
    return $request->user();
})->middleware('auth:sanctum');

// RFID API Routes (tanpa auth untuk hardware RFID)
Route::prefix('rfid')->group(function () {
    Route::post('/scan', [RfidController::class, 'scan']);
    Route::get('/status', [RfidController::class, 'status']);

    // RFID Registration Routes
    Route::post('/register', [RfidRegistrationController::class, 'register']);
    Route::post('/scan-for-registration', [RfidRegistrationController::class, 'scanForRegistration']);
    Route::get('/users', [RfidRegistrationController::class, 'getUsers']);
});
