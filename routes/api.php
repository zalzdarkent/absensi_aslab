<?php

use App\Http\Controllers\Api\RfidController;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;

Route::get('/user', function (Request $request) {
    return $request->user();
})->middleware('auth:sanctum');

// RFID API Routes (tanpa auth untuk hardware RFID)
Route::prefix('rfid')->group(function () {
    Route::post('/scan', [RfidController::class, 'scan']);
    Route::get('/status', [RfidController::class, 'status']);
});
