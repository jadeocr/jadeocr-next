<template>
  <div class="dashboard">
    <div class="grid grid-cols-1 md:grid-cols-4">
      <div class="col-span-1">
        <sidebar/>
      </div>
      <div class="mt-12 col-span-1 page-content">
        <div class="pl-1 mx-6 text-xl font-normal lg:text-2xl xl:text-3xl">
          Dashboard
        </div>
      </div>
    </div> 
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import Sidebar from '../components/Sidebar.vue'

  export default defineComponent({
    name: 'Dashboard',
    components: {
      Sidebar,
    },
  })
</script>

