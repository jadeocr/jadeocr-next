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
              Classes
            </div>
            <div class="mt-4">
              <form class="w-1/3 lg:w-1/4" @submit.prevent="">
                <input
                  v-model="classCode"
                  class="w-full py-2 leading-tight text-gray-200 shadow appearance-none border-underline focus:outline-none focus:shadow-outline-none"
                  :placeholder="'Enter your six-digit class code'"
                />
                <button
                  @click="callJoinClass()"
                  class="px-3 py-2 mt-6 font-light rounded bg-nord2"
                  type="submit"
                >
                  Join Class
                </button>
                <span class="mx-4 font-normal text-nord11">
                  {{ $store.state.classes.classErrMsg }}
                </span>
              </form>
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
    name: 'Classes',
    components: {
      Sidebar,
    },
    data() {
      return {
        classCode: "",
      }
    },
    methods: {
      callJoinClass(): void {
        this.$store.dispatch('classes/joinClass', this.classCode)
      },
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
