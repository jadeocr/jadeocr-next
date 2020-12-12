<template>
  <div class="dashboard">
    <div class="grid grid-cols-1 md:grid-cols-4">
      <div class="col-span-1">
        <sidebar />
      </div>
      <div class="mt-12 overflow-y-auto md:mt-16 col-span-1 page-content">
        <div class="mx-6 md:mx-4 lg:mx-10 xl:mx-20">
          <div class="text-2xl font-normal md:text-3xl">
            Dashboard
          </div>
          <div class="mt-4 text-lg">
            <button
              @click="changeMenuType('learn')"
              :class="{ 'opacity-75': menuType != 'learn' }"
            >
              My Decks
            </button>
            |
            <button
              @click="changeMenuType('assigned')"
              :class="{ 'opacity-75': menuType != 'assigned' }"
            >
              Assigned
            </button>
          </div>
          <div class="mt-8">
            <router-link
              :to="{ name: 'DeckCreate' }"
              class="px-3 py-2 mt-8 rounded bg-nord2"
            >
              Create Deck
            </router-link>
            <deck-menu :menuType="menuType" class="mt-2 md:mt-12" />
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import Sidebar from '../components/Sidebar.vue'
  import DeckMenu from '../components/DeckMenu.vue'

  export default defineComponent({
    name: 'Dashboard',
    components: {
      Sidebar,
      DeckMenu,
    },
    data() {
      return {
        menuType: 'learn',
      }
    },
    methods: {
      changeMenuType(menuType: string): void {
        this.menuType = menuType
      },
    },
    mounted() {
      this.$store.dispatch('decks/fetchDecks')
      /* this.$store.dispatch('decks/fetchAssignedDecks') */
    },
  })
</script>
