<template>
  <div class="dashboard">
    <div class="grid grid-cols-1 md:grid-cols-4">
      <div class="col-span-1">
        <sidebar />
      </div>
      <div class="mt-12 md:mt-16 col-span-1 page-content">
        <div
          class="mx-6 text-2xl font-normal md:mx-4 md:text-3xl lg:mx-10 xl:mx-20"
        >
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
    mounted() {
      this.$store.dispatch('decks/fetchDecks')
    }
  })
</script>
