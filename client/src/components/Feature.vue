<template>
  <div class="row-span-1" :class="{ 'mt-12 md:mt-16': '!isFirstElement' }">
    <div class="flex items-center justify-center font-semibold">
      <FeatureIcons :name="name" />
      <p class="ml-5">{{ title }}</p>
    </div>
    <p class="mt-2 font-normal text-left text-center text-md">{{ body }}</p>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import FeatureIcons from './FeatureIcons.vue'

  export default defineComponent({
    components: {
      FeatureIcons,
    },
    props: {
      title: String,
      name: String,
      body: String,
      isFirstElement: Boolean,
    },
  })
</script>

<style scoped></style>
