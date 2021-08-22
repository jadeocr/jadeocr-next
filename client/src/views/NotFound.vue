<template>
  <div id="not-found" class="flex flex-col items-center justify-center">
    <div class="lg:grid lg:grid-cols-2 lg:mt-16 xl:-mt-12 md:-mt-12">
      <img
        :src="require('@/assets/title-image.png')"
        alt="An image of the Chinese character 玉"
        class="lg:col-span-1"
      />
      <div
        class="flex flex-col justify-center -mt-8 text-3xl font-normal text-center md:text-5xl lg:col-span-1 lg:text-left md:-mt-32 lg:-mt-8"
      >
        <p class="leading-normal">囧</p>
        <p class="leading-normal">404 Not Found</p>
        <router-link
          :to="{ name: 'Home' }"
          class="w-1/2 px-4 py-2 mx-auto mt-4 text-xl text-center rounded md:text-3xl hoverlink bg-nord2 lg:w-48 lg:mx-0"
        >
          Home 返回
        </router-link>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'

  export default defineComponent({
    name: 'NotFound',
  })
</script>
