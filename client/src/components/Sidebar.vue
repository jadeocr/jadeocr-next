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
      class="p-12 -ml-4 text-lg rounded md:py-24 xl:mt-10 xl:px-16 md:mt-10 lg:mt-auto"
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
            <p class="ml-5">Dashboard</p>
          </div>
        </router-link>
      </div>
      <div class="my-16">
        <router-link :to="{ name: 'Classes' }">
          <div class="flex items-center">
            <svg
              xmlns="http://www.w3.org/2000/svg"
              width="1.25em"
              fill="currentColor"
              class="bi bi-pencil-square"
              viewBox="0 0 16 16"
            >
              <path
                d="M15.502 1.94a.5.5 0 0 1 0 .706L14.459 3.69l-2-2L13.502.646a.5.5 0 0 1 .707 0l1.293 1.293zm-1.75 2.456l-2-2L4.939 9.21a.5.5 0 0 0-.121.196l-.805 2.414a.25.25 0 0 0 .316.316l2.414-.805a.5.5 0 0 0 .196-.12l6.813-6.814z"
              />
              <path
                fill-rule="evenodd"
                d="M1 13.5A1.5 1.5 0 0 0 2.5 15h11a1.5 1.5 0 0 0 1.5-1.5v-6a.5.5 0 0 0-1 0v6a.5.5 0 0 1-.5.5h-11a.5.5 0 0 1-.5-.5v-11a.5.5 0 0 1 .5-.5H9a.5.5 0 0 0 0-1H2.5A1.5 1.5 0 0 0 1 2.5v11z"
              />
            </svg>
            <p class="ml-5">Classes</p>
          </div>
        </router-link>
      </div>
      <div class="my-16">
        <router-link :to="{ name: 'Public' }">
          <div class="flex items-center">
            <svg
              xmlns="http://www.w3.org/2000/svg"
              width="1.25em"
              fill="currentColor"
              class="bi bi-globe"
              viewBox="0 0 16 16"
            >
              <path
                d="M0 8a8 8 0 1 1 16 0A8 8 0 0 1 0 8zm7.5-6.923c-.67.204-1.335.82-1.887 1.855A7.97 7.97 0 0 0 5.145 4H7.5V1.077zM4.09 4a9.267 9.267 0 0 1 .64-1.539 6.7 6.7 0 0 1 .597-.933A7.025 7.025 0 0 0 2.255 4H4.09zm-.582 3.5c.03-.877.138-1.718.312-2.5H1.674a6.958 6.958 0 0 0-.656 2.5h2.49zM4.847 5a12.5 12.5 0 0 0-.338 2.5H7.5V5H4.847zM8.5 5v2.5h2.99a12.495 12.495 0 0 0-.337-2.5H8.5zM4.51 8.5a12.5 12.5 0 0 0 .337 2.5H7.5V8.5H4.51zm3.99 0V11h2.653c.187-.765.306-1.608.338-2.5H8.5zM5.145 12c.138.386.295.744.468 1.068.552 1.035 1.218 1.65 1.887 1.855V12H5.145zm.182 2.472a6.696 6.696 0 0 1-.597-.933A9.268 9.268 0 0 1 4.09 12H2.255a7.024 7.024 0 0 0 3.072 2.472zM3.82 11a13.652 13.652 0 0 1-.312-2.5h-2.49c.062.89.291 1.733.656 2.5H3.82zm6.853 3.472A7.024 7.024 0 0 0 13.745 12H11.91a9.27 9.27 0 0 1-.64 1.539 6.688 6.688 0 0 1-.597.933zM8.5 12v2.923c.67-.204 1.335-.82 1.887-1.855.173-.324.33-.682.468-1.068H8.5zm3.68-1h2.146c.365-.767.594-1.61.656-2.5h-2.49a13.65 13.65 0 0 1-.312 2.5zm2.802-3.5a6.959 6.959 0 0 0-.656-2.5H12.18c.174.782.282 1.623.312 2.5h2.49zM11.27 2.461c.247.464.462.98.64 1.539h1.835a7.024 7.024 0 0 0-3.072-2.472c.218.284.418.598.597.933zM10.855 4a7.966 7.966 0 0 0-.468-1.068C9.835 1.897 9.17 1.282 8.5 1.077V4h2.355z"
              />
            </svg>
            <p class="ml-5">Public</p>
          </div>
        </router-link>
      </div>
      <div class="my-16">
        <router-link :to="{ name: 'Stats' }">
          <div class="flex items-center">
            <svg
              xmlns="http://www.w3.org/2000/svg"
              width="1.25em"
              fill="currentColor"
              class="bi bi-graph-up"
              viewBox="0 0 16 16"
            >
              <path
                fill-rule="evenodd"
                d="M0 0h1v15h15v1H0V0zm10 3.5a.5.5 0 0 1 .5-.5h4a.5.5 0 0 1 .5.5v4a.5.5 0 0 1-1 0V4.9l-3.613 4.417a.5.5 0 0 1-.74.037L7.06 6.767l-3.656 5.027a.5.5 0 0 1-.808-.588l4-5.5a.5.5 0 0 1 .758-.06l2.609 2.61L13.445 4H10.5a.5.5 0 0 1-.5-.5z"
              />
            </svg>
            <p class="ml-5">Stats</p>
          </div>
        </router-link>
      </div>
      <button @click="callSignOut" class="px-3 py-2 rounded bg-nord9">
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
      callSignOut() {
        this.$store.dispatch('auth/signOut')
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
