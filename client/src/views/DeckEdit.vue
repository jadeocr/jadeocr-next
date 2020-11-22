<template>
  <div id="deck-edit">
    <div class="dashboard">
      <div class="grid grid-cols-1 md:grid-cols-4">
        <div class="col-span-1">
          <sidebar />
        </div>
        <div class="col-span-1">
          <div class="mt-12 md:mt-16 col-span-1 page-content">
            <div class="mx-6 md:mx-4 lg:mx-10 xl:mx-20">
              <div class="text-2xl font-normal md:text-3xl">
                {{ name }}
              </div>
              <!-- TODO: Add Editing grid -->
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import Sidebar from '../components/Sidebar.vue'

  interface Deck {
    characters: Array<string>
    readonly id: string
    title: string
    description: string
    readonly creatorID: string
    readonly creatorFirst: string
    readonly creatorLast: string
    isPublic: false
  }

  export default defineComponent({
    name: 'DeckEdit',
    components: {
      Sidebar,
    },
    props: {
      name: {
        type: String,
        required: false, // left null if on create
      },
    },
    data() {
      return {
        deck: Array<Deck>(),
      }
    },
  })
</script>

<style></style>
