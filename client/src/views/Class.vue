<template>
  <div id="class">
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
            <div class="mt-10 col-span-1">
              <!-- TODO: Change to assigned decks -->
              <div v-if="!$store.state.classes.currClassAssignments.length">
                <div
                  class="mt-8 grid grid-cols-1 lg:grid-cols-2 xl:grid-cols-3"
                >
                  <div
                    class="px-12 py-8 rounded bg-nord1 lg:col-span-2 xl:col-span-2"
                  >
                    No decks have been assigned for this class yet!
                    <span v-if="$store.state.auth.isTeacher">
                      Go to "Manage Class" to assign one!
                    </span>
                    <span v-else>
                      Check back later!
                    </span>
                  </div>
                </div>
              </div>
              <div
                v-else-if="$store.state.classes.currClassAssignments.length"
                class="grid grid-cols-1 md:grid-cols-2"
              >
                <div
                  v-for="(n, deck) in $store.state.classes
                    .currClassAssignments"
                  :key="deck.key"
                  class="my-4 mr-4 col-span-1"
                >
                  <div class="p-8 text-center md:p-12 bg-nord10 rounded-md">
                    <router-link
                      class="text-xl font-normal"
                      :to="{
                        path: `/deck/${$store.state.classes.currClassAssignments[deck].deckId}`,
                      }"
                    >
                      {{
                        $store.state.classes.currClassAssignments[deck]
                          .deckName
                      }}
                    </router-link>
                    <div>
                      {{
                        $store.state.classes.currClassAssignments[deck]
                          .deckDescription
                      }}
                    </div>
                  </div>
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
              <div v-if="$store.state.auth.isTeacher" class="mt-4">
                Join code:
                {{ currClass.classCode }}
              </div>
              <div class="mt-8">
                <router-link
                  v-if="$store.state.auth.isTeacher"
                  class="px-4 py-2 rounded bg-nord9"
                  :to="{
                    path: `/class/${currClass.classCode}/edit`,
                  }"
                >
                  Manage Class
                </router-link>
              </div>
              <div class="mt-8">
                <div>
                  <button
                    v-if="!$store.state.auth.isTeacher"
                    @click="toggleModalVisibility()"
                    class="mb-4 font-normal opacity-75"
                  >
                    Leave Class
                  </button>
                </div>
                <button
                  class="px-4 py-2 rounded bg-nord2"
                  @click="$router.push({ name: 'Classes' })"
                >
                  Go Back
                </button>
              </div>
              <modal
                headline="Confirm leaving class"
                confirmBtnTxt="Leave"
                v-if="modalIsVisible"
                @confirm="callLeaveClass()"
                @exit-modal="toggleModalVisibility()"
              >
                Are you sure you want to leave the class {{ currClass.name }}?
              </modal>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
  import Modal from '../components/Modal.vue'
  import Sidebar from '../components/Sidebar.vue'
  import { ClassI } from '../interfaces/Class'
  import { defineComponent } from 'vue'

  export default defineComponent({
    name: 'Class',
    props: {
      classCode: {
        type: String,
        required: true,
      },
    },
    data() {
      return {
        currClass: {} as ClassI,
        modalIsVisible: false,
      }
    },
    methods: {
      toggleModalVisibility(): void {
        this.modalIsVisible = !this.modalIsVisible
      },
      callLeaveClass(): void {
        this.$store.dispatch('classes/leaveClass', this.classCode)
        this.toggleModalVisibility()
      },
    },
    components: {
      Sidebar,
      Modal,
    },
    mounted() {
      // find name of class
      // note: can store in order of class code and do a binary search for
      // O(log n) time instead of O(n)
      const allClasses = this.$store.state.classes.classes.concat(
        this.$store.state.classes.classesTeaching
      )
      for (let i = 0; i < allClasses.length; i++) {
        if (allClasses[i].classCode == this.classCode) {
          this.currClass = allClasses[i]
        }
      }

      if (this.$store.state.auth.isTeacher) {
        this.$store.dispatch(
          'classes/getAssignedDecksAsTeacher',
          this.classCode
        )
      } else {
        this.$store.dispatch(
          'classes/getAssignedDecksAsStudent',
          this.classCode
        )
      }
    },
  })
</script>

<style scoped></style>
