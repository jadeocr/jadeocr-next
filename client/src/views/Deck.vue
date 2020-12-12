<template>
  <div id="deck-edit">
    <div class="dashboard">
      <div class="grid grid-cols-1 md:grid-cols-4">
        <div class="col-span-1">
          <sidebar />
        </div>
        <div class="mt-12 overflow-y-auto md:mt-16 col-span-1 page-content">
          <div class="mx-6 md:mx-4 lg:mx-10 xl:mx-20">
            <div class="text-2xl font-normal md:text-3xl">
              {{ deck.title }}
            </div>
            <div class="mt-1 text-lg font-normal md:text-xl text-nord5">
              {{ deck.description }}
            </div>
            <div class="mt-4 text-lg">
              <div class="mt-4 text-lg">
                <router-link
                  class="font-normal opacity-75"
                  :to="{ path: `/deckedit/edit/${deck._id}` }"
                >
                  Edit Deck
                </router-link>
              </div>
              <div>

              </div>
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

  interface Character {
    // readonly _id: string // Mongo generated
    readonly id: string // UUID generated, need to change server to use _id later
    char: string
    definition: string
    pinyin: string
  }

  interface Deck {
    characters: Array<Character>
    readonly _id: string
    title: string
    description: string
    readonly creatorID: string
    readonly creatorFirst: string
    readonly creatorLast: string
    isPublic: boolean
  }

  export default defineComponent({
    name: 'Deck',
    components: {
      Sidebar
    },
    props: {
      id: {
        type: String,
        required: true,
      }
    },
    data() {
      return {
        deck: {} as Deck
      }
    },
    methods: {
      findDeck(): Deck {
        return this.$store.state.decks.decks.find((deck: Deck) => {
          return deck._id == this.id
        })
      },
    },
    mounted() {
      this.deck = this.findDeck()
    }
  })
</script>

<style scoped>
</style>
