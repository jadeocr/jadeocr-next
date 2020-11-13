<template>
  <div class="dashboard">
    <sidebar/>
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
