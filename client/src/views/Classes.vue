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
              <form class="w-3/4 md:w-1/2 lg:w-1/3" @submit.prevent="">
                <input
                  v-model="classCode"
                  class="w-full py-2 leading-tight text-gray-200 shadow appearance-none border-underline focus:outline-none focus:shadow-outline-none"
                  :placeholder="'Enter your six-digit class code'"
                />
                <button
                  @click="callJoinClass()"
                  class="px-3 py-2 my-6 font-light rounded bg-nord2"
                  type="submit"
                >
                  Join Class
                </button>
                <button v-if="$store.state.auth.isTeacher"
                  @click="callCreateClass()"
                  class="px-3 py-2 mx-4 my-6 font-light rounded bg-nord2"
                  type="submit"
                >
                  New Class
                </button>
                <span class="mx-4 font-normal text-nord11">
                  {{ $store.state.classes.classErrMsg }}
                </span>
              </form>
            </div>
            <div
              v-if="$store.state.classes.classes.length"
              class="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-3"
            >
              <div
                v-for="(n, c) in $store.state.classes.classes"
                :key="c.key"
                class="my-4 mr-4 col-span-1"
              >
                <div class="p-8 text-center md:p-12 bg-nord10 rounded-md">
                  <router-link
                    class="text-xl font-normal"
                    :to="{
                      path: `/class/${$store.state.classes.classes[c].classCode}`,
                    }"
                  >
                    {{ $store.state.classes.classes[c].name }}
                  </router-link>
                  <div>{{ $store.state.classes.classes[c].description }}</div>
                </div>
              </div>
            </div>
            <div
              v-if="$store.state.classes.classesTeaching.length"
              class="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-3"
            >
              <div
                v-for="(n, c) in $store.state.classes.classesTeaching"
                :key="c.key"
                class="my-4 mr-4 col-span-1"
              >
                <div class="p-8 text-center md:p-12 bg-nord10 rounded-md">
                  <router-link
                    class="text-xl font-normal"
                    :to="{
                      path: `/class/${$store.state.classes.classesTeaching[c].classCode}`,
                    }"
                  >
                    {{ $store.state.classes.classesTeaching[c].name }}
                  </router-link>
                  <div>
                    {{ $store.state.classes.classesTeaching[c].description }}
                  </div>
                </div>
              </div>
            </div>
            <div v-else>
              <div class="mt-8 grid grid-cols-1 lg:grid-cols-2 xl:grid-cols-3">
                <div
                  class="px-12 py-8 rounded bg-nord1 lg:col-span-2 xl:col-span-2"
                >
                  Join 
                  {{ $store.state.auth.isTeacher ? 'or create' : ''}}
                  a class to see it appear here!
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

  export default defineComponent({
    name: 'Classes',
    components: {
      Sidebar,
    },
    data() {
      return {
        classCode: '',
        className: 'Development Class',
        classDescription: 'A class meant for testing and dev purposes',
      }
    },
    methods: {
      callJoinClass(): void {
        this.$store.dispatch('classes/joinClass', this.classCode)
      },
      callCreateClass(): void {
        this.$store.dispatch('classes/createClass', {
          className: this.className,
          description: this.classDescription,
        })
      }
    },
    mounted() {
      this.$store.commit('classes/setClassErrMsg', '')
      this.$store.dispatch('classes/getClassesJoined')
      this.$store.dispatch('classes/getClassesTeaching')
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
