<template>
  <div id="deck-edit">
    <div class="dashboard">
      <div class="grid grid-cols-1 md:grid-cols-4">
        <div class="col-span-1">
          <sidebar />
        </div>
        <div class="mt-12 overflow-y-auto md:mt-16 col-span-1 page-content">
          <div class="mx-6 md:mx-4 lg:mx-10 xl:mx-20">
            <div class="text-2xl font-normal text-center md:text-3xl md:text-left">
              {{ deck.title }}
            </div>
            <div class="mt-1 text-lg font-normal text-center md:text-xl text-nord5 md:text-left">
              {{ deck.description }}
            </div>
            <div class="grid grid-cols-1 md:grid-cols-2">
              <div class="mt-10 col-span-1">
                <div class="text-xl font-normal text-center md:text-2xl md:text-left">
                  Learn
                </div>
                <div class="mt-6 text-center md:mt-8 md:text-left">
                  <button class="px-3 py-2 mr-2 rounded bg-nord10">
                    Flashcards
                  </button>
                  <button class="px-3 py-2 rounded lg:mx-4 bg-nord10">
                    Handwriting
                  </button>
                  <button class="px-3 py-2 mx-2 rounded lg:mx-2 md:mx-0 md:my-4 lg:my-0 bg-nord10">
                    Quiz
                  </button>
                  <div class="mt-12 text-center lg:mt-12 md:text-left">
                    <p class="mb-6 text-xl font-normal">Vocabulary</p>
                    <table class="border border-white xl:mb-12 lg:w-3/4">
                      <tr v-for="(n, i) in deck.characters" :key="i.key" class="py-2">
                        <td class="px-2 py-4 text-lg font-normal xl:px-4 chinese">
                          {{ i+1 }}.
                        </td>
                        <td class="px-4 py-4 text-lg font-normal chinese">
                          {{ deck.characters[i].char }}
                        </td>
                        <td class="px-4 lg:px-8 chinese">
                          {{ deck.characters[i].pinyin }}
                        </td>
                        <td class="px-4 lg:px-8 chinese">
                          {{ deck.characters[i].definition }}
                        </td>
                      </tr>
                    </table>
                  </div>
                </div>
              </div> 
              <div class="mt-10 text-center md:pr-12 md:text-right xl:mx-48 xl:w-1/3 col-span-1">
                <div class="text-xl font-normal md:text-2xl">
                  Deck Info
                </div>
                <div class="mt-8">
                  Created by {{ deck.creatorFirst + ' ' + deck.creatorLast }}
                </div>
                <div class="mt-4">
                  This deck is {{ deck.isPublic ? 'public' : 'private' }}
                </div>
                <div class="mt-8 text-lg">
                  <router-link
                    class="font-normal opacity-75"
                    :to="{ path: `/deckedit/edit/${deck._id}` }"
                  >
                    Edit Deck
                  </router-link>
                </div>
                <div class="mt-10">
                  <button
                    class="px-4 py-2 rounded bg-nord2"
                    @click="$router.go(-1)"
                  >
                    Go Back
                  </button>
                </div>
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
