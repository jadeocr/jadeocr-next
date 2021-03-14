<template>
  <div id="deck-edit">
    <div class="dashboard">
      <div class="grid grid-cols-1 md:grid-cols-4">
        <div class="col-span-1">
          <sidebar />
        </div>
        <LoadingIcon v-if="isLoading" />
        <div
          class="mt-12 overflow-y-auto md:mt-16 col-span-1 page-content"
          v-else
        >
          <div class="mx-6 md:mx-4 lg:mx-10 xl:mx-20">
            <div
              class="text-2xl font-normal text-center md:text-3xl md:text-left"
            >
              {{ $store.state.decks.currDeck.title }}
            </div>
            <div
              class="mt-1 text-lg font-normal text-center md:text-xl text-nord5 md:text-left"
            >
              {{ $store.state.decks.currDeck.description }}
            </div>
            <div class="grid grid-cols-1 md:grid-cols-2">
              <div class="mt-10 col-span-1">
                <div
                  class="text-xl font-normal text-center md:text-2xl md:text-left"
                >
                  Learn
                </div>
                <div class="mt-6 text-center md:mt-8 md:text-left">
                  <div>
                    <select
                      class="px-4 py-2 rounded bg-nord10"
                      v-model="learnMode"
                    >
                      <option selected disabled value=""
                        >Select a Learn Mode</option
                      >
                      <option value="learn"> Learn </option>
                      <option value="flashcards"> Flashcards </option>
                      <option value="ocr"> Handwriting </option>
                      <option value="quiz"> Quiz </option>
                    </select>
                    <button
                      @click="launchLearn(learnMode)"
                      class="px-4 py-2 mx-4 font-normal rounded bg-nord10"
                    >
                      Go!
                    </button>
                  </div>
                  <div class="mt-12 text-center lg:mt-12 md:text-left">
                    <p class="mb-6 text-xl font-normal">Vocabulary</p>
                    <table class="xl:mb-12 lg:w-3/4">
                      <tr
                        v-for="(n, i) in $store.state.decks.currDeck.characters"
                        :key="i.key"
                        class="py-2"
                      >
                        <td
                          class="py-4 pr-2 text-lg font-normal xl:pr-4 chinese"
                        >
                          {{ i + 1 }}.
                        </td>
                        <td class="px-4 py-4 text-lg font-normal chinese">
                          {{ $store.state.decks.currDeck.characters[i].char }}
                        </td>
                        <td class="px-4 lg:px-8 chinese">
                          {{ $store.state.decks.currDeck.characters[i].pinyin }}
                        </td>
                        <td class="px-4 lg:px-8 chinese">
                          {{
                            $store.state.decks.currDeck.characters[i].definition
                          }}
                        </td>
                      </tr>
                    </table>
                  </div>
                </div>
              </div>
              <div
                class="mt-10 text-center md:pr-8 md:text-right xl:mx-32 col-span-1"
              >
                <div class="text-xl font-normal md:text-2xl">
                  Deck Info
                </div>
                <div class="mt-8">
                  <div>Created by</div>
                  {{
                    $store.state.decks.currDeck.creatorFirst +
                      ' ' +
                      $store.state.decks.currDeck.creatorLast
                  }}
                </div>
                <div class="mt-4">
                  <!-- This deck is {{ $store.state.decks.currDeck.access.isPublic ? 'public' : 'private' }} -->
                  <!--
                    This is the worst piece of code I have ever seen
                    I am thankful to say that I did not write this
                    If something with deck publicity breaks on this page, it's
                    probably here. Specifically decks always shown as private.
                    哎呀
                    –Tanay
                  -->
                  This deck is
                  {{
                    JSON.stringify($store.state.decks.currDeck).substring(
                      22,
                      26
                    ) == 'true'
                      ? 'public'
                      : 'private'
                  }}
                </div>
                <div class="mt-8 text-lg">
                  <router-link
                    class="font-normal opacity-75"
                    :to="{
                      path: `/deckedit/edit/${$store.state.decks.currDeck._id}`,
                    }"
                  >
                    Edit Deck
                  </router-link>
                </div>
                <div class="mt-10">
                  <button class="px-4 py-2 rounded bg-nord2" @click="goBack()">
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
  import { RouteLocationNormalized } from 'vue-router'
  import Sidebar from '../components/Sidebar.vue'
  import LoadingIcon from '../components/LoadingIcon.vue'
  import { Deck } from '../interfaces/Deck'

  export default defineComponent({
    name: 'Deck',
    components: {
      Sidebar,
      LoadingIcon,
    },
    props: {
      id: {
        type: String,
        required: true,
      },
    },
    data() {
      return {
        learnMode: '',
        prevRoute: '',
        isLoading: false,
      }
    },
    methods: {
      launchLearn(type: string): void {
        this.$router.push({
          path: `/review/${type}/${this.id}/${this.$store.state.decks.currDeck.title}`,
        })
      },
      goBack() {
        if (this.prevRoute == 'DeckEdit') {
          this.$router.push({ name: 'Dashboard' })
        } else {
          this.$router.go(-1)
        }
      },
    },
    async mounted() {
      this.isLoading = true
      await this.$store.dispatch('decks/fetchCards', this.id)
      this.isLoading = false
    },
    beforeRouteEnter(
      to: RouteLocationNormalized,
      from: RouteLocationNormalized,
      next: Function
    ) {
      // eslint-disable-next-line
      next((vm: any) => {
        vm.prevRoute = from.name
      })
    },
  })
</script>

<style scoped></style>
