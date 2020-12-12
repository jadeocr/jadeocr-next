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
                {{ deck.title }}
              </div>
              <!-- Editing Grid -->
              <div class="my-6 md:w-1/3">
                <input v-model='deck.description' class="w-full py-2 leading-tight text-gray-200 shadow appearance-none focus:outline-none focus:shadow-outline-none" type="text" placeholder='Description'>
              </div>
              <div>
                <div class="mt-8">
                  <button class='px-4 py-2 mr-4 rounded bg-nord2' @click="addCard(1)">
                    <svg class="bi bi-plus" width="1.25em" height="1.25em" viewBox="0 0 16 16" fill="currentColor" xmlns="http://www.w3.org/2000/svg">
                      <path fill-rule="evenodd" d="M8 3.5a.5.5 0 01.5.5v4a.5.5 0 01-.5.5H4a.5.5 0 010-1h3.5V4a.5.5 0 01.5-.5z" clip-rule="evenodd"/>
                      <path fill-rule="evenodd" d="M7.5 8a.5.5 0 01.5-.5h4a.5.5 0 010 1H8.5V12a.5.5 0 01-1 0V8z" clip-rule="evenodd"/>
                    </svg>
                  </button>
                  <button class='px-4 py-2 mr-4 rounded bg-nord2' @click="addCard(-1)">
                    <svg class="bi bi-dash" width="1.25em" height="1.25em" viewBox="0 0 16 16" fill="currentColor" xmlns="http://www.w3.org/2000/svg">
                      <path fill-rule="evenodd" d="M3.5 8a.5.5 0 01.5-.5h8a.5.5 0 010 1H4a.5.5 0 01-.5-.5z" clip-rule="evenodd"/>
                    </svg>
                  </button>
                </div>
              </div>
              <div class="mt-12">
                <div v-for="(n, i) in deck.characters" :key="i.key">
                  <form class="flex flex-wrap my-6 -mx-4">
                    <div class="w-1/3 px-4 lg:w-1/4">
                      <input v-model="deck.characters[i].char" class="w-full py-2 leading-tight text-gray-200 shadow appearance-none chinese focus:outline-none focus:shadow-outline-none" :placeholder="(i+1)+'. 学'">
                    </div>
                    <div class="w-1/3 px-4 lg:w-1/4">
                      <input v-model="deck.characters[i].pinyin" class="w-full py-2 leading-tight text-gray-200 shadow appearance-none focus:outline-none focus:shadow-outline-none" :placeholder="(i+1)+'. xué'">
                    </div>
                    <div class="w-1/3 px-4 lg:w-1/4">
                      <input v-model="deck.characters[i].definition" class="w-full py-2 leading-tight text-gray-200 shadow appearance-none focus:outline-none focus:shadow-outline-none" :placeholder="(i+1)+'. to study'">
                    </div>
                  </form>
                </div>
                <div class="mt-12">
                  <button v-if="id" class="px-3 py-2 rounded bg-nord10" @click="callUpdateDeck()">
                    Save Changes
                  </button>
                  <button v-else class="px-3 py-2 rounded bg-nord10" @click="callCreateDeck()">
                    Create Deck
                  </button>
                  <div>
                    <button v-if="id" class="px-4 py-2 my-4 rounded bg-nord11" @click="callDeleteDeck()">
                      Delete Deck
                    </button>
                  </div>
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
    isPublic: false
  }

  export default defineComponent({
    name: 'DeckEdit',
    components: {
      Sidebar,
    },
    props: {
      id: {
        type: String,
        required: false, // left null if on create
      },
    },
    data() {
      return {
        deck: {} as Deck,
      }
    },
    methods: {
      findDeck(): Deck {
        return this.$store.state.decks.decks.find(( deck: Deck ) => {
          return deck._id == this.id
        })
      },
      addCard(num: number): void {
        if (num == -1) {
          this.deck.characters.splice(this.deck.characters.length - 1)
        } else if (num == 1) {
          this.deck.characters.push({
            id: '',
            char: '',
            pinyin: '',
            definition: ''
          } as Character)
        } else {
          throw new Error('Number to add to deck is not valid!')
        }
      },
      callCreateDeck(): void {
        this.$store.dispatch('decks/createDeck', this.deck)
      },
      callUpdateDeck(): void {
        this.$store.dispatch('decks/updateDeck', this.deck)
      },
      callDeleteDeck(): void {
        this.$store.dispatch('decks/deleteDeck', this.deck)
      }
    },
    mounted() {
      this.deck = this.findDeck()
    }
  })
</script>

<style scoped>
input {
  background-color: transparent;
  border-bottom: 2px solid rgba(255,255,255,0.4);
  transition: border-bottom 0.25s ease-in-out;
}
input:focus {
  border-bottom: 2px solid rgba(255,255,255,1);
}
</style>

