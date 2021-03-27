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
            <div class="text-2xl font-normal md:text-3xl">
              Classes
            </div>
            <div class="mt-2">
              <button
                @click="toggleJoinModalVisibility()"
                class="px-3 py-2 my-6 font-light rounded bg-nord2"
                type="submit"
              >
                Join Class
              </button>
              <button
                v-if="$store.state.auth.isTeacher"
                @click="toggleCreateModalVisibility()"
                class="px-3 py-2 mx-4 my-6 font-light rounded bg-nord2"
                type="submit"
              >
                New Class
              </button>
              <span class="mx-4 font-normal text-nord11">
                {{ $store.state.classes.classErrMsg }}
              </span>
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
            <div v-else-if="!$store.state.classes.classes.length">
              <div class="mt-8 grid grid-cols-1 lg:grid-cols-2 xl:grid-cols-3">
                <div
                  class="px-12 py-8 rounded bg-nord1 lg:col-span-2 xl:col-span-2"
                >
                  Join
                  {{ $store.state.auth.isTeacher ? 'or create' : '' }}
                  a class to see it appear here!
                </div>
              </div>
            </div>
          </div>
          <modal
            headline="Join Class"
            confirmBtnTxt="Join"
            v-if="joinModalIsVisible"
            modalType="form"
            @confirm="handleJoinModalConfirm()"
            @exit-modal="toggleJoinModalVisibility()"
          >
            <form class="pt-2 md:w-11/12" @submit.prevent="">
              <input
                v-model="classCode"
                class="w-full py-2 my-4 leading-tight text-gray-200 shadow appearance-none border-underline focus:outline-none focus:shadow-outline-none"
                placeholder="123456"
              />
              <span>
                Enter the 6-digit class code from your teacher
              </span>
            </form>
          </modal>
          <modal
            headline="Create Class"
            confirmBtnTxt="Create"
            v-if="createModalIsVisible"
            modalType="form"
            @confirm="handleCreateModalConfirm()"
            @exit-modal="toggleCreateModalVisibility()"
          >
            <form class="pt-4 md:w-11/12" @submit.prevent="">
              <input
                v-model="className"
                class="w-full py-2 my-4 leading-tight text-gray-200 shadow appearance-none border-underline focus:outline-none focus:shadow-outline-none"
                :placeholder="'Class Name'"
              />
              <input
                v-model="classDescription"
                class="w-full py-2 mb-6 leading-tight text-gray-200 shadow appearance-none border-underline focus:outline-none focus:shadow-outline-none"
                :placeholder="'Class Description'"
              />
              <span>
                Note: These cannot be changed later.
              </span>
            </form>
          </modal>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import Sidebar from '../components/Sidebar.vue'
  import Modal from '../components/Modal.vue'
  import LoadingIcon from '../components/LoadingIcon.vue'

  export default defineComponent({
    name: 'Classes',
    components: {
      Sidebar,
      Modal,
      LoadingIcon,
    },
    data() {
      return {
        classCode: '',
        className: '',
        classDescription: '',
        joinModalIsVisible: false,
        createModalIsVisible: false,
        isLoading: false,
      }
    },
    methods: {
      toggleJoinModalVisibility(): void {
        this.joinModalIsVisible = !this.joinModalIsVisible
      },
      toggleCreateModalVisibility(): void {
        this.createModalIsVisible = !this.createModalIsVisible
      },
      handleCreateModalConfirm(): void {
        this.$store.dispatch('classes/createClass', {
          className: this.className,
          description: this.classDescription,
        })
        this.toggleCreateModalVisibility()
      },
      handleJoinModalConfirm(): void {
        this.$store.dispatch('classes/joinClass', this.classCode)
        this.toggleJoinModalVisibility()
        this.$store.dispatch('classes/getClassesJoined')
      },
    },
    async mounted() {
      this.isLoading = true
      this.$store.commit('classes/setClassErrMsg', '')
      await this.$store.dispatch('classes/getClassesJoined')
      await this.$store.dispatch('classes/getClassesTeaching')
      this.isLoading = false
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
