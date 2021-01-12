<template>
  <div id="class-edit">
    <div class="grid grid-cols-1 md:grid-cols-4">
      <div class="col-span-1">
        <sidebar />
      </div>
      <div class="mt-12 overflow-y-auto md:mt-16 col-span-1 page-content">
        <div class="mx-6 md:mx-4 lg:mx-10 xl:mx-20">
          <div
            class="text-2xl font-normal text-center md:text-3xl md:text-left"
          >
            {{ currClass.name }}
          </div>
          <div
            class="mt-1 text-lg font-normal text-center md:text-xl text-nord5 md:text-left"
          >
            {{ currClass.description }}
          </div>
          <div class="grid grid-cols-1 md:grid-cols-2">
            <div class="col-span-1">
              <div class="mt-6 text-lg">
                <button
                  @click="changeMenuType('decks')"
                  :class="{ 'opacity-75': menuType != 'decks' }"
                >
                  Decks
                </button>
                |
                <button
                  @click="changeMenuType('students')"
                  :class="{ 'opacity-75': menuType != 'students' }"
                >
                  Students
                </button>
                |
                <button
                  @click="changeMenuType('stats')"
                  :class="{ 'opacity-75': menuType != 'stats' }"
                >
                  Stats
                </button>
              </div>
              <div class="mt-10">
                <div v-if="menuType == 'decks'">
                  Decks
                </div>
                <div v-if="menuType == 'students'">
                  <div v-for="(n, i) in currClass.students" :key="i.key">
                    {{ i+1 + '. ' + currClass.students[i].firstName + ' ' + currClass.students[i].lastName }}
                  </div>
                </div>
                <div v-if="menuType == 'stats'">
                  Stats
                </div>
              </div>
            </div>
            <div
              class="mt-10 text-center md:pr-8 md:text-right xl:mx-48 xl:w-1/3 col-span-1"
            >
              <div class="text-xl font-normal md:text-2xl">
                Class Info
              </div>
              <div class="mt-8">
                {{
                  $store.state.auth.firstName +
                    ' ' +
                    $store.state.auth.lastName ==
                  currClass.teacherName
                    ? 'You are the teacher of this class'
                    : currClass.teacherName
                }}
              </div>
              <div class="mt-8">
                <div>
                  <button
                    v-if="$store.state.auth.isTeacher"
                    @click="toggleModalVisibility()"
                    class="mb-4 font-normal opacity-75"
                  >
                    Delete Class
                  </button>
                </div>
                <button
                  class="px-4 py-2 rounded bg-nord2"
                  @click="$router.go(-1)"
                >
                  Cancel
                </button>
              </div>
              <modal
                headline="Confirm class deletion"
                confirmBtnTxt="Delete"
                v-if="modalIsVisible"
                @confirm="callDeleteClass()"
                @exit-modal="toggleModalVisibility()"
              >
                Are you sure you want to permanently delete the class {{ currClass.name }}?
              </modal>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>  
</template>

<script lang="ts">
  import axios from 'axios'
  import Sidebar from '../components/Sidebar.vue'
  import Modal from '../components/Modal.vue'
  import { ClassI } from '../interfaces/Class'
  import { defineComponent } from 'vue'
  const apiBaseURL = process.env.VUE_APP_API_BASEURL

  export default defineComponent({
    name: 'ClassEdit',
    props: {
      classCode: {
        type: String,
        required: true,
      },
    },
    components: {
      Sidebar,
      Modal,
    },
    data() {
      return {
        currClass: {} as ClassI,
        modalIsVisible: false,
        menuType: 'decks',
      }
    },
    methods: {
      toggleModalVisibility(): void {
        this.modalIsVisible = !this.modalIsVisible
      },
      callDeleteClass(): void {
        this.$store.dispatch('classes/deleteClass', this.classCode)
        this.toggleModalVisibility()
      },
      getClassDetails(): void {
        axios({
          method: 'post',
          withCredentials: true,
          url: `${apiBaseURL}/class/class`,
          data: {
            classCode: this.classCode,
          }
        })  
        .then((res) => {
          console.log(res.data)
          this.currClass = res.data
        })
        .catch((err) => {
          console.log(err)
        })
      },
      changeMenuType(menuType: string): void {
        this.menuType = menuType
      },
    },
    mounted() {
      this.getClassDetails()
    },
  })
</script>

<style scoped></style>
