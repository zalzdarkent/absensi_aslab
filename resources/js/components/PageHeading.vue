<template>
  <div class="mb-8">
    <h1 class="text-2xl font-bold text-gray-900">{{ title }}</h1>
    <p v-if="description" class="mt-1 text-sm text-gray-600">{{ description }}</p>
  </div>
</template>

<script setup lang="ts">
defineProps({
  title: String,
  description: String
})
</script>
