<template>
  <div id="deck-edit">
    <div class="dashboard">
      <div class="grid grid-cols-1 md:grid-cols-4">
        <div class="col-span-1">
          <sidebar />
        </div>
        <div class="col-span-1">
          <div class="mt-12 overflow-y-auto md:mt-16 page-content">
            <div class="mx-6 md:mx-4 lg:mx-10 xl:mx-20">
              <input
                v-model="$store.state.decks.currDeck.title"
                class="w-full py-2 text-2xl font-normal md:text-3xl"
                type="text"
                placeholder="Title"
              />
              <!-- Editing Grid -->
              <div class="my-6 md:w-1/3">
                <input
                  v-model="$store.state.decks.currDeck.description"
                  class="w-full py-2 leading-tight text-gray-200 shadow appearance-none border-underline focus:outline-none focus:shadow-outline-none"
                  type="text"
                  placeholder="Description"
                />
              </div>
              <div>
                <div class="mt-8">
                  <button
                    class="px-4 py-2 mr-4 rounded bg-nord2"
                    @click="addCard(1)"
                  >
                    <svg
                      class="bi bi-plus"
                      width="1.25em"
                      height="1.25em"
                      viewBox="0 0 16 16"
                      fill="currentColor"
                      xmlns="http://www.w3.org/2000/svg"
                    >
                      <path
                        fill-rule="evenodd"
                        d="M8 3.5a.5.5 0 01.5.5v4a.5.5 0 01-.5.5H4a.5.5 0 010-1h3.5V4a.5.5 0 01.5-.5z"
                        clip-rule="evenodd"
                      />
                      <path
                        fill-rule="evenodd"
                        d="M7.5 8a.5.5 0 01.5-.5h4a.5.5 0 010 1H8.5V12a.5.5 0 01-1 0V8z"
                        clip-rule="evenodd"
                      />
                    </svg>
                  </button>
                  <button
                    class="px-4 py-2 mr-4 rounded bg-nord2"
                    @click="addCard(-1)"
                  >
                    <svg
                      class="bi bi-dash"
                      width="1.25em"
                      height="1.25em"
                      viewBox="0 0 16 16"
                      fill="currentColor"
                      xmlns="http://www.w3.org/2000/svg"
                    >
                      <path
                        fill-rule="evenodd"
                        d="M3.5 8a.5.5 0 01.5-.5h8a.5.5 0 010 1H4a.5.5 0 01-.5-.5z"
                        clip-rule="evenodd"
                      />
                    </svg>
                  </button>
                </div>
                <div
                  v-if="$store.state.decks.decksErrorMsg"
                  class="mt-4 font-normal text-nord11"
                >
                  {{ $store.state.decks.decksErrorMsg }}
                </div>
              </div>
              <div class="mt-12">
                <div v-for="(n, i) in $store.state.decks.currDeck.characters" :key="i.key">
                  <form class="flex flex-wrap my-6 -mx-4">
                    <div class="w-1/3 px-4 lg:w-1/4">
                      <input
                        @change="callGetPinyinDefinition(i)"
                        v-model="$store.state.decks.currDeck.characters[i].char"
                        class="w-full py-2 leading-tight text-gray-200 shadow appearance-none border-underline chinese focus:outline-none focus:shadow-outline-none"
                        :placeholder="i + 1 + '. 学'"
                      />
                    </div>
                    <div class="w-1/3 px-4 lg:w-1/4">
                      <input
                        v-model="$store.state.decks.currDeck.characters[i].pinyin"
                        class="w-full py-2 leading-tight text-gray-200 shadow appearance-none border-underline focus:outline-none focus:shadow-outline-none"
                        :placeholder="i + 1 + '. xué'"
                      />
                    </div>
                    <div class="w-1/3 px-4 lg:w-1/4">
                      <input
                        v-model="$store.state.decks.currDeck.characters[i].definition"
                        class="w-full py-2 leading-tight text-gray-200 shadow appearance-none border-underline focus:outline-none focus:shadow-outline-none"
                        :placeholder="i + 1 + '. to study'"
                      />
                    </div>
                  </form>
                </div>
                <div class="mt-20">
                  <button
                    class="px-4 py-2 mr-4 rounded bg-nord2"
                    @click="$router.go(-1)"
                  >
                    Cancel
                  </button>
                  <button
                    v-if="id"
                    class="px-3 py-2 mr-4 rounded bg-nord10"
                    @click="callUpdateDeck()"
                  >
                    Save Changes
                  </button>
                  <button
                    v-else
                    class="px-3 py-2 mr-4 rounded bg-nord10"
                    @click="callCreateDeck()"
                  >
                    Create Deck
                  </button>
                  <div class="mt-8 mb-16">
                    <button
                      class="px-4 py-2 mr-4 rounded bg-nord2"
                      @click="togglePublic()"
                    >
                      <div v-if="$store.state.decks.currDeck.isPublic">Public</div>
                      <div v-else>Private</div>
                    </button>
                    <button
                      v-if="id"
                      class="px-4 py-2 mr-4 rounded bg-nord11"
                      @click="toggleModalVisibility()"
                    >
                      Delete Deck
                    </button>
                    <modal
                      headline="Confirm deck deletion"
                      v-if="modalIsVisible"
                      @confirm-delete="callDeleteDeck()"
                      @exit-modal="toggleModalVisibility()"
                    >
                      Are you sure you want to permanently delete the deck
                      {{ $store.state.decks.currDeck.deckName }}?
                    </modal>
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
  import Modal from '../components/Modal.vue'

  interface Character {
    // readonly _id: string // Mongo generated
    readonly id: string // UUID generated, need to change server to use _id later
    char: string
    definition: string
    pinyin: string
  }

  interface Deck {
    characters: Array<Character>
    readonly deckId: string
    title: string
    description: string
    readonly creatorID: string
    readonly creatorFirst: string
    readonly creatorLast: string
    isPublic: boolean
  }

  export default defineComponent({
    name: 'DeckEdit',
    components: {
      Sidebar,
      Modal,
    },
    props: {
      id: {
        type: String,
        required: false, // left null if on create
      },
    },
    data() {
      return {
        modalIsVisible: false,
      }
    },
    methods: {
      addCard(num: number): void {
        if (num == -1) {
          this.$store.state.decks.currDeck.characters.splice(this.$store.state.decks.currDeck.characters.length - 1)
        } else if (num == 1) {
          this.$store.state.decks.currDeck.characters.push({
            id: '',
            char: '',
            pinyin: '',
            definition: '',
          } as Character)
        } else {
          throw new Error('Number to add to deck is not valid!')
        }
      },
      togglePublic(): void {
        this.$store.state.decks.currDeck.access.isPublic = !this.$store.state.decks.currDeck.access.isPublic
      },
      callCreateDeck(): void {
        this.$store.dispatch('decks/createDeck', this.$store.state.decks.currDeck)
      },
      callUpdateDeck(): void {
        this.$store.dispatch('decks/updateDeck', this.$store.state.decks.currDeck)
      },
      callDeleteDeck(): void {
        this.$store.dispatch('decks/deleteDeck', this.$store.state.decks.currDeck)
        this.toggleModalVisibility()
      },
      toggleModalVisibility(): void {
        this.modalIsVisible = !this.modalIsVisible
      },
      callGetPinyinDefinition(i: number): void {
        this.$store.dispatch('decks/getPinyinDefinition', {
          deck: this.$store.state.decks.currDeck,
          charIndex: i
        })
      }
    },
    mounted() {
      this.$store.commit('decks/setDeckErrMsg', '')
      console.log(this.id)
      if (this.$store.state.decks.currDeck.deckId == this.id) {
        this.$store.dispatch('decks/fetchCards', this.id)
      } else if (!this.id) {
        this.$store.commit('decks/setCurrDeck', {
          title: '',
          description: '',
          characters: Array<Character>(),
          isPublic: false,
        } as Deck)
        this.addCard(1)
      }
    },
  })
</script>

<style scoped>
  input {
    background-color: transparent;
    outline-width: 0;
  }
  input.border-underline {
    border-bottom: 2px solid rgba(255, 255, 255, 0.4);
    transition: border-bottom 0.25s ease-in-out;
  }
  input.border-underline:focus {
    border-bottom: 2px solid rgba(255, 255, 255, 1);
  }
</style>
