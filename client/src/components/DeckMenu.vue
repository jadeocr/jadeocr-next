<template>
  <div id="deck-menu">
    <div class="flex flex-col justify-center w-1/2 mx-auto -mt-40 text-2xl text-center md:text-3xl col-span-1 page-content" v-if="isLoading">
      Loading...
    </div>
    <!-- Show personal decks -->
    <div
      v-else-if="
        (!$store.state.decks.decks.length && menuType == 'all') ||
          (!$store.state.decks.decksCreated.length && menuType == 'created')
      "
    >
      <div class="mt-8 grid grid-cols-1 lg:grid-cols-2 xl:grid-cols-3">
        <div class="px-12 py-8 rounded bg-nord1 lg:col-span-2 xl:col-span-2">
          Create a deck to get started!
        </div>
      </div>
    </div>
    <div
      v-else-if="$store.state.decks.decks.length && menuType == 'all'"
      class="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-3"
    >
      <div
        v-for="(n, deck) in $store.state.decks.decks"
        :key="deck.key"
        class="my-4 mr-4 col-span-1"
      >
        <div class="p-8 text-center md:p-12 bg-nord10 rounded-md">
          <router-link
            class="text-xl font-normal"
            :to="{
              path: `/deck/${$store.state.decks.decks[deck].deckId}`,
            }"
          >
            {{ $store.state.decks.decks[deck].deckName }}
          </router-link>
          <div>{{ $store.state.decks.decks[deck].deckDescription }}</div>
        </div>
      </div>
    </div>
    <div
      v-else-if="
        $store.state.decks.decksCreated.length && this.menuType == 'created'
      "
      class="grid grid-cols0 md:grid-cols-2 xl:grid-cols-3"
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
      v-else-if="!$store.state.decks.decksAssigned.length && menuType == 'assigned'"
    >
      <div class="mt-8 grid grid-cols-1 lg:grid-cols-2 xl:grid-cols-3">
        <div class="px-12 py-8 rounded bg-nord1 lg:col-span-2 xl:col-span-2">
          Join a class to view assigned decks!
        </div>
      </div>
    </div>
    <div
      v-else-if="
        $store.state.decks.decksAssigned.length && menuType == 'assigned'
      "
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
          return ['assigned', 'all', 'created'].includes(value)
        },
      },
    },
    data() {
      return {
        isLoading: false,
      }
    },
    watch: {
      menuType() {
        this.fetchDecks()
      },
    },
    methods: {
      async fetchDecks(): Promise<any> {
        this.isLoading = true
        if (this.menuType == 'all') {
          await this.$store.dispatch('decks/fetchDecks')
        } else if (this.menuType == 'assigned') {
          await this.$store.dispatch('decks/fetchAssignedDecks')
        } else if (this.menuType == 'created') {
          await this.$store.dispatch('decks/fetchCreatedDecks')
        }
        this.isLoading = false
      },
    },
    async created() {
      this.fetchDecks()
    },
  })
</script>

<style scoped></style>
