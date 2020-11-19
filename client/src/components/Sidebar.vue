<template>
  <div id="sidebar">
    <div class="mx-4 md:mx-8 sidebar-button md:hidden">
      <button
        @click="collapse"
        class="px-2 py-1 text-white rounded btn-purple opacity-87 hover:opacity-75"
      >
        <svg
          class="bi bi-list"
          width="1.5em"
          viewBox="0 0 16 16"
          fill="currentColor"
          xmlns="http://www.w3.org/2000/svg"
        >
          <path
            fill-rule="evenodd"
            d="M2.5 11.5A.5.5 0 013 11h10a.5.5 0 010 1H3a.5.5 0 01-.5-.5zm0-4A.5.5 0 013 7h10a.5.5 0 010 1H3a.5.5 0 01-.5-.5zm0-4A.5.5 0 013 3h10a.5.5 0 010 1H3a.5.5 0 01-.5-.5z"
            clip-rule="evenodd"
          />
        </svg>
      </button>
    </div>
    <div
      class="p-12 -ml-4 text-lg rounded md:py-24 xl:mt-24 xl:px-16 md:mt-24 lg:mt-auto"
      id="sidebar-content"
    >
      <div class="my-8">
        <router-link :to="{ name: 'Dashboard' }">
          <div class="flex items-center">
            <svg
              class="bi bi-book"
              width="1.25em"
              viewBox="0 0 16 16"
              fill="currentColor"
              xmlns="http://www.w3.org/2000/svg"
            >
              <path
                fill-rule="evenodd"
                d="M3.214 1.072C4.813.752 6.916.71 8.354 2.146A.5.5 0 018.5 2.5v11a.5.5 0 01-.854.354c-.843-.844-2.115-1.059-3.47-.92-1.344.14-2.66.617-3.452 1.013A.5.5 0 010 13.5v-11a.5.5 0 01.276-.447L.5 2.5l-.224-.447.002-.001.004-.002.013-.006a5.017 5.017 0 01.22-.103 12.958 12.958 0 012.7-.869zM1 2.82v9.908c.846-.343 1.944-.672 3.074-.788 1.143-.118 2.387-.023 3.426.56V2.718c-1.063-.929-2.631-.956-4.09-.664A11.958 11.958 0 001 2.82z"
                clip-rule="evenodd"
              />
              <path
                fill-rule="evenodd"
                d="M12.786 1.072C11.188.752 9.084.71 7.646 2.146A.5.5 0 007.5 2.5v11a.5.5 0 00.854.354c.843-.844 2.115-1.059 3.47-.92 1.344.14 2.66.617 3.452 1.013A.5.5 0 0016 13.5v-11a.5.5 0 00-.276-.447L15.5 2.5l.224-.447-.002-.001-.004-.002-.013-.006-.047-.023a12.582 12.582 0 00-.799-.34 12.96 12.96 0 00-2.073-.609zM15 2.82v9.908c-.846-.343-1.944-.672-3.074-.788-1.143-.118-2.387-.023-3.426.56V2.718c1.063-.929 2.631-.956 4.09-.664A11.956 11.956 0 0115 2.82z"
                clip-rule="evenodd"
              />
            </svg>
            <p class="ml-5">Learn</p>
          </div>
        </router-link>
      </div>
      <div class="my-16">
        <router-link :to="{ name: 'Dashboard' }">
          <div class="flex items-center">
            <svg
              class="bi bi-book"
              width="1.25em"
              viewBox="0 0 16 16"
              fill="currentColor"
              xmlns="http://www.w3.org/2000/svg"
            >
              <path
                fill-rule="evenodd"
                d="M3.214 1.072C4.813.752 6.916.71 8.354 2.146A.5.5 0 018.5 2.5v11a.5.5 0 01-.854.354c-.843-.844-2.115-1.059-3.47-.92-1.344.14-2.66.617-3.452 1.013A.5.5 0 010 13.5v-11a.5.5 0 01.276-.447L.5 2.5l-.224-.447.002-.001.004-.002.013-.006a5.017 5.017 0 01.22-.103 12.958 12.958 0 012.7-.869zM1 2.82v9.908c.846-.343 1.944-.672 3.074-.788 1.143-.118 2.387-.023 3.426.56V2.718c-1.063-.929-2.631-.956-4.09-.664A11.958 11.958 0 001 2.82z"
                clip-rule="evenodd"
              />
              <path
                fill-rule="evenodd"
                d="M12.786 1.072C11.188.752 9.084.71 7.646 2.146A.5.5 0 007.5 2.5v11a.5.5 0 00.854.354c.843-.844 2.115-1.059 3.47-.92 1.344.14 2.66.617 3.452 1.013A.5.5 0 0016 13.5v-11a.5.5 0 00-.276-.447L15.5 2.5l.224-.447-.002-.001-.004-.002-.013-.006-.047-.023a12.582 12.582 0 00-.799-.34 12.96 12.96 0 00-2.073-.609zM15 2.82v9.908c-.846-.343-1.944-.672-3.074-.788-1.143-.118-2.387-.023-3.426.56V2.718c1.063-.929 2.631-.956 4.09-.664A11.956 11.956 0 0115 2.82z"
                clip-rule="evenodd"
              />
            </svg>
            <p class="ml-5">Learn</p>
          </div>
        </router-link>
      </div>
      <div class="my-16">
        <router-link :to="{ name: 'Dashboard' }">
          <div class="flex items-center">
            <svg
              class="bi bi-book"
              width="1.25em"
              viewBox="0 0 16 16"
              fill="currentColor"
              xmlns="http://www.w3.org/2000/svg"
            >
              <path
                fill-rule="evenodd"
                d="M3.214 1.072C4.813.752 6.916.71 8.354 2.146A.5.5 0 018.5 2.5v11a.5.5 0 01-.854.354c-.843-.844-2.115-1.059-3.47-.92-1.344.14-2.66.617-3.452 1.013A.5.5 0 010 13.5v-11a.5.5 0 01.276-.447L.5 2.5l-.224-.447.002-.001.004-.002.013-.006a5.017 5.017 0 01.22-.103 12.958 12.958 0 012.7-.869zM1 2.82v9.908c.846-.343 1.944-.672 3.074-.788 1.143-.118 2.387-.023 3.426.56V2.718c-1.063-.929-2.631-.956-4.09-.664A11.958 11.958 0 001 2.82z"
                clip-rule="evenodd"
              />
              <path
                fill-rule="evenodd"
                d="M12.786 1.072C11.188.752 9.084.71 7.646 2.146A.5.5 0 007.5 2.5v11a.5.5 0 00.854.354c.843-.844 2.115-1.059 3.47-.92 1.344.14 2.66.617 3.452 1.013A.5.5 0 0016 13.5v-11a.5.5 0 00-.276-.447L15.5 2.5l.224-.447-.002-.001-.004-.002-.013-.006-.047-.023a12.582 12.582 0 00-.799-.34 12.96 12.96 0 00-2.073-.609zM15 2.82v9.908c-.846-.343-1.944-.672-3.074-.788-1.143-.118-2.387-.023-3.426.56V2.718c1.063-.929 2.631-.956 4.09-.664A11.956 11.956 0 0115 2.82z"
                clip-rule="evenodd"
              />
            </svg>
            <p class="ml-5">Learn</p>
          </div>
        </router-link>
      </div>
      <button @click="signOut" class="px-3 py-2 rounded bg-nord9">
        Sign Out
      </button>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import JQuery from 'jquery'
  const $ = JQuery

  export default defineComponent({
    name: 'Sidebar',
    methods: {
      signOut() {
        this.$store.dispatch('signOutAction')
      },
      collapse() {
        const currPos = $('#sidebar-content').css('left')
        if (currPos == '0px') {
          $('#sidebar-content').css('left', '-100vw')
          $('.page-content').css('width', '100vw')
          $('.page-content').css('opacity', '100%')
          $('.page-content').css('visibility', 'visible')
        } else {
          $('#sidebar-content').css('left', '0px')
          $('.page-content').css('width', '0px')
          $('.page-content').css('opacity', '0%')
          $('.page-content').css('visibility', 'hidden')
        }
      },
    },
  })
</script>

<style scoped>
  #sidebar-content {
    position: fixed;
    left: 0px;
    width: 300px;
    -webkit-transition: left 0.6s ease;
    transition: left 0.6s ease;
  }

  @media (max-width: 640px) {
    #sidebar-content {
      width: 100vw;
      left: -100vw;
    }
  }

  #sidebar-button {
    position: fixed;
    left: 20px;
    top: 20px;
  }
</style>
