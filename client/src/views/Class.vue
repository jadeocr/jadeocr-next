<template>
  <div id="class">
    <div class="grid grid-cols-1 md:grid-cols-4">
      <div class="col-span-1">
        <sidebar />
      </div>
      <div class="mt-12 overflow-y-auto md:mt-16 col-span-1 page-content">
      </div>
    </div>
  </div>
</template>


<script lang="ts">
  import { defineComponent } from 'vue'
  import Sidebar from '../components/Sidebar.vue'

  export default defineComponent({
    name: 'Class',
    props: {
      id: {
        type: String,
        required: true,
      }
    },
    components: {
      Sidebar,
    },
  })
</script>


<style scoped>

</style>
