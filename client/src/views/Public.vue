<template>
  <div id="login">
    <div class="grid grid-cols-1 md:grid-cols-4">
      <div class="col-span-1">
        <sidebar />
      </div>
      <div class="mt-12 overflow-y-auto md:mt-16 col-span-1 page-content">
        <div class="mx-6 md:mx-4 lg:mx-10 xl:mx-20">
          <div class="text-2xl font-normal md:text-3xl">
            Public
          </div>
          <div class="mt-4 text-lg">
            <div class="w-1/3">
              <form @submit.prevent="">
                <input
                  v-model="query"
                  class="w-4/5 py-2 leading-tight text-gray-200 shadow appearance-none border-underline focus:outline-none focus:shadow-outline-none"
                  placeholder="Search"
                />
                <span class="float-right p-3 font-bold">
                  <button @click="callPublicFromQuery()">
                    <svg xmlns="http://www.w3.org/2000/svg" width="1.125em" fill="currentColor" viewBox="0 0 16 16">
                      <path d="M11.742 10.344a6.5 6.5 0 1 0-1.397 1.398h-.001c.03.04.062.078.098.115l3.85 3.85a1 1 0 0 0 1.415-1.414l-3.85-3.85a1.007 1.007 0 0 0-.115-.1zM12 6.5a5.5 5.5 0 1 1-11 0 5.5 5.5 0 0 1 11 0z"/>
                    </svg>
                  </button>
                </span>
              </form>
            </div>
            <div v-for="deck in $store.state.decks.publicDecks" :key="deck.key">
              {{ deck.title }}
              {{ deck.description }}
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

  export default defineComponent({
    name: 'Public',
    components: {
      Sidebar
    },
    data() {
      return {
        query: '',
      }
    },
    methods: {
      callPublicFromQuery(query: string): void {
        this.$store.dispatch('decks/publicFromQuery', this.query)
      }
    }
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
