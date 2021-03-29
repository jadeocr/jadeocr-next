<template>
  <div id="login">
    <div class="grid grid-cols-1 md:grid-cols-4">
      <div class="col-span-1">
        <sidebar />
      </div>
      <div class="mt-12 overflow-y-auto md:mt-16 col-span-1 page-content">
        <div class="mx-6 md:mx-4 lg:mx-10 xl:mx-20">
          <div class="text-2xl font-normal md:text-3xl">
            Profile
          </div>
          <div class="mt-4 text-lg">
            <button
              @click="changeMenuType('stats')"
              :class="{ 'opacity-75': menuType != 'stats' }"
            >
              Stats
            </button>
            |
            <button
              @click="changeMenuType('settings')"
              :class="{ 'opacity-75': menuType != 'settings' }"
            >
              Settings
            </button>
          </div>
          <div class="mt-12">
            <div
              v-if="menuType == 'stats'"
              class="px-12 py-8 rounded bg-nord1 lg:col-span-2 xl:col-span-2"
            >
              <div class="font-bold">
                {{ $store.state.auth.firstName + ' ' + $store.state.auth.lastName}}
              </div>
              <div>
                {{ $store.state.auth.isTeacher ? 'Teacher Account' : 'Student Account' }}
              </div>
              <div class="mt-2 leading-relaxed">
                <div>{{ $store.state.auth.stats.decksTotal }} decks total</div>
                <div>{{ $store.state.auth.stats.decksOwned }} decks owned</div>
                <div>Currently teaching {{ $store.state.auth.stats.classesTeaching }} classes</div>
                <div>Currently in {{ $store.state.auth.stats.classesJoined }} classes</div>
              </div>
            </div>
            <div
              v-else-if="menuType == 'settings'"
              class="lg:col-span-2 xl:col-span-2"
            >
              <button :class="['w-64', 'px-4', 'py-2', 'font-normal', 'text-center', 'rounded', 'hoverlink', $store.state.auth.isTeacher ? 'bg-nord11' : 'bg-nord15']" @click="callToggleTeacher()">
                {{ $store.state.auth.isTeacher ? 'Deactivate Teacher Features' : 'Activate Teacher Features'}} 
              </button>
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
    name: 'Profile',
    components: {
      Sidebar,
    },
    data() {
      return {
        menuType: 'stats',
      }
    },
    methods: {
      changeMenuType(menuType: string): void {
        this.menuType = menuType
      },
      callToggleTeacher() {
        this.$store.dispatch('auth/toggleTeacher', !this.$store.state.auth.isTeacher)
      },
    },
    mounted() {
      this.$store.dispatch('auth/getStats')
    },
  })
</script>
