<template>
  <div id="deck-menu">
    <!-- Show personal decks -->
    <div v-if="!$store.state.decks.decksCreated.length && (menuType == 'learn' || menuType == 'all')">
      <div class="mt-8 grid grid-cols-1 lg:grid-cols-2 xl:grid-cols-3">
        <div class="px-12 py-8 rounded bg-nord1 lg:col-span-2 xl:col-span-2">
          Create a deck to get started!
        </div>
      </div>
    </div>
    <div
      v-else-if="$store.state.decks.decksCreated.length && menuType == 'learn'"
      class="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-3"
    >
      <div
        v-for="(n, deck) in $store.state.decks.decksCreated"
        :key="deck.key"
        class="my-4 mr-4 col-span-1"
      >
        <div class="p-8 text-center md:p-12 bg-nord10 rounded-md">
          <router-link
            class="text-xl font-normal"
            :to="{
              path: `/deck/${$store.state.decks.decksCreated[deck].deckId}`,
            }"
          >
            {{ $store.state.decks.decksCreated[deck].deckName }}
          </router-link>
          <div>{{ $store.state.decks.decksCreated[deck].deckDescription }}</div>
        </div>
      </div>
    </div>

    <!-- Show assigned decks -->
    <div
      v-if="!$store.state.decks.decksAssigned.length && menuType == 'assigned'"
    >
      <div class="mt-8 grid grid-cols-1 lg:grid-cols-2 xl:grid-cols-3">
        <div class="px-12 py-8 rounded bg-nord1 lg:col-span-2 xl:col-span-2">
          Join a class to view assigned decks!
        </div>
      </div>
    </div>
    <div
      v-if="$store.state.decks.decksAssigned.length && menuType == 'assigned'"
      class="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-3"
    >
      <div
        v-for="(n, deck) in $store.state.decks.decksAssigned"
        :key="deck.key"
        class="my-4 mr-4 col-span-1"
      >
        <div class="p-8 text-center md:p-12 bg-nord10 rounded-md">
          <div class="text-xl font-normal">
            {{ $store.state.decks.decksAssigned[deck].title }}
          </div>
          <div>{{ $store.state.decks.decksAssigned[deck].description }}</div>
          <div>
            {{ $store.state.decks.decksAssigned[deck].creatorFirst }}
            {{ $store.state.decks.decksAssigned[deck].creatorLast }}
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'

  export default defineComponent({
    name: 'DeckMenu',
    props: {
      menuType: {
        type: String,
        required: true,
        validator: (value: string): boolean => {
          return ['learn', 'assigned', 'all'].includes(value)
        },
      },
    },
    mounted() {
      this.$store.dispatch('decks/fetchCreatedDecks')
      this.$store.dispatch('decks/fetchAssignedDecks')
    },
  })
</script>

<style scoped></style>
