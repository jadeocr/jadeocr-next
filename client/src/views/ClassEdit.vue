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
              <div class="mt-8">
                <div v-if="menuType == 'decks'">
                  <div>
                    <div class="text-2xl font-normal">
                      Assign Deck
                    </div>
                    <div class="mt-4">
                      <div>
                        <select
                          v-model="assignDeckProperties.deckId"
                          class="px-4 py-1 rounded bg-nord7"
                        >
                          <option selected disabled value=""
                            >Select a Deck</option
                          >
                          <option
                            v-for="deck in $store.state.decks.decksCreated"
                            :value="deck.deckId"
                            :key="deck.key"
                          >
                            {{ deck.deckName }}
                          </option>
                        </select>
                      </div>
                      <div class="my-4">
                        <select
                          v-model="assignDeckProperties.mode"
                          class="px-4 py-1 rounded bg-nord7"
                        >
                          <option selected disabled value=""
                            >Select a Review Mode</option
                          >
                          <option value="learn">Learn</option>
                          <option value="quiz">Quiz</option>
                          <option value="srs">SRS</option>
                        </select>
                        <select
                          v-model="assignDeckProperties.front"
                          class="px-4 py-1 mx-4 rounded bg-nord7"
                        >
                          <option selected disabled value=""
                            >Select Card Front</option
                          >
                          <option value="pinyin">Pinyin</option>
                          <option value="character">Character</option>
                          <option value="definition">Definition</option>
                          <option
                            v-if="assignDeckProperties.mode != 'quiz'"
                            value="handwriting"
                            >Handwriting</option
                          >
                        </select>
                      </div>
                      <div class="my-4">
                        <span>
                          <input
                            v-model="assignDeckProperties.repetitions"
                            class="w-1/12 py-1 text-center border-underline"
                            type="number"
                            min="1"
                          />
                          {{
                            assignDeckProperties.repetitions == 1
                              ? 'Repetition'
                              : 'Repetitions'
                          }}
                        </span>
                        <span v-if="assignDeckProperties.mode == 'learn'">
                          <input
                            class="ml-4"
                            type="checkbox"
                            id="scramble-checkbox"
                            v-model="assignDeckProperties.scramble"
                          />
                          <label class="mx-2" for="scramble-checkbox"
                            >Scramble</label
                          >
                        </span>
                      </div>
                      <div class="my-4">
                        <label for="dueDate">Due Date: </label>
                        <input
                          v-model="assignDeckProperties.dueDate"
                          type="date"
                          id="dueDate"
                          name="dueDate"
                          min="1970-01-01"
                          class="px-2 py-1 mx-4 rounded bg-nord7"
                        />
                      </div>
                      <div class="my-6" @click="callAssignDeck">
                        <button class="px-4 py-2 rounded bg-nord7">
                          Assign Deck
                        </button>
                      </div>
                    </div>
                  </div>
                  <div class="mt-8">
                    <div class="text-2xl font-normal">
                      Assigned Decks
                    </div>
                    <div v-if="!$store.state.classes.currClassAssignments">
                      <div
                        class="px-12 py-8 my-4 rounded bg-nord1 lg:col-span-2 xl:col-span-2"
                      >
                        Get started by assigning a deck to this class!
                      </div>
                    </div>
                    <div v-else class="grid grid-cols-1 md:grid-cols-2">
                      <div
                        v-for="(n, deck) in $store.state.classes.currClassAssignments"
                        :key="deck.key"
                        class="my-4 mr-4"
                      >
                        <div class="grid grid-cols-3">
                          <div class="col-span-1">
                            <router-link
                              class="text-lg font-normal"
                              :to="{
                                path: `/deck/${$store.state.classes.currClassAssignments[deck].deckId}`,
                              }"
                            >
                              {{
                                deck +
                                  1 +
                                  '. ' +
                                  $store.state.classes.currClassAssignments[deck].deckName
                              }}
                            </router-link>
                            <div>
                              {{ $store.state.classes.currClassAssignments[deck].deckDescription }}
                            </div>
                          </div>
                          <div class="col-span-2">
                            <button class="px-4 py-2 rounded bg-nord9" @click="callUnassignDeck($store.state.classes.currClassAssignments[deck].deckId)">
                              Unassign
                            </button>
                          </div>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
                <div v-if="menuType == 'students'">
                  <div v-if="!currClass.students.length">
                    <div
                      class="px-12 py-8 rounded bg-nord1 lg:col-span-2 xl:col-span-2"
                    >
                      Share your class code to your students!
                    </div>
                  </div>
                  <div v-for="(n, i) in currClass.students" :key="i.key">
                    {{
                      i +
                        1 +
                        '. ' +
                        currClass.students[i].firstName +
                        ' ' +
                        currClass.students[i].lastName
                    }}
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
              <div v-if="$store.state.auth.isTeacher" class="mt-4">
                Join code:
                {{ currClass.classCode }}
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
                  Go Back
                </button>
              </div>
              <modal
                headline="Confirm class deletion"
                confirmBtnTxt="Delete"
                v-if="modalIsVisible"
                @confirm="callDeleteClass()"
                @exit-modal="toggleModalVisibility()"
              >
                Are you sure you want to permanently delete the class
                {{ currClass.name }}?
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
  import { AssignDeckData } from '../interfaces/AssignDeckData'
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
        currClass: {
          classCode: 'Loading...',
          description: 'Loading...',
          name: 'Loading...',
          teacherName: 'Loading...',
          students: [],
          assignedDecks: [],
        } as ClassI,
        modalIsVisible: false,
        menuType: 'decks',
        assignDeckProperties: {
          mode: 'learn',
          front: 'pinyin',
          scramble: null,
          repetitions: 1,
          dueDate: new Date(), // placeholder, UNIX timestamp in ms
          classCode: this.classCode,
          deckId: '',
        } as AssignDeckData,
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
          },
        })
          .then((res) => {
            this.currClass = res.data
          })
          .catch((err) => {
            console.log(err)
          })
      },
      changeMenuType(menuType: string): void {
        this.menuType = menuType
      },
      callAssignDeck(): void {
        // epoch time in ms
        this.assignDeckProperties.dueDate = new Date(
          this.assignDeckProperties.dueDate
        ).valueOf()
        this.$store.dispatch('classes/assignDeck', this.assignDeckProperties)
      },
      callUnassignDeck(deckId: string, assignmentId: string): void {
        this.$store.dispatch('classes/unassignDeck', { // TODO: Extract into type and specify in store
          classCode: this.currClass.classCode,
          deckId: deckId,
          assignmentId: assignmentId
        }) // TODO: Fill params
      },
    },
    created() {
      this.getClassDetails()
    },
  })
</script>

<style scoped>
  input.border-underline {
    background-color: transparent;
    outline-width: 0;
    border-bottom: 2px solid rgba(255, 255, 255, 0.4);
    transition: border-bottom 0.25s ease-in-out;
  }
  input.border-underline:focus {
    border-bottom: 2px solid rgba(255, 255, 255, 1);
  }
</style>
