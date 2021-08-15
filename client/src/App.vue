<template>
    <div class="p-4 text-center text-gray-200 bg-gray-800">
      This website is still in beta, but we're working hard to improve the experience
    </div>
  <start-link linkType="navigation" class="mt-6" />
  <router-view />
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import StartLink from './components/StartLink.vue'

  // Allows TypeScript type inference
  export default defineComponent({
    name: 'App',
    components: {
      StartLink,
    },
  })
</script>

<style>
  @import './assets/css/tailwind.css';
  @import url('https://fonts.googleapis.com/css2?family=Open+Sans:wght@300;400;600&display=swap');
  @import url('https://fonts.googleapis.com/css2?family=Noto+Sans+SC:wght@100;300;400&display=swap');

  body {
    background-size: cover;
    background: #121212;
  }
  #app {
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    font-family: 'Open Sans', sans-serif;
    font-weight: 300;
    color: #ffffff;
  }

  .chinese {
    font-family: 'Noto Sans SC', sans-serif;
  }

  .hoverlink:hover {
    opacity: 0.8;
  }


  /* Slidable content for dashboard pages */
  .page-content {
    position: fixed;
    right: 0px;
    width: calc(100vw - 200px);
    -webkit-transition: width 0.6s ease;
    transition: width 0.6s ease;
    height: calc(98vh - 8.5rem);
    -webkit-transition: width 0.6s ease, opacity 0.2s ease, visibility 0.2s ease;
    transition: width 0.6s ease, opacity 0.2s ease, visibility 0.2s ease;
  }
  @media (max-width: 1280px) {
    .page-content {
      height: 80vh;
    }
  }
  @media (max-width: 1024px) {
    .page-content {
      height: 75vh;
    }
  }
  @media (max-width: 768px) {
    .page-content {
      height: 75vh;
    }
  }
  @media (max-width: 640px) {
    .page-content {
      height: 72.5vh;
      width: 100vw;
    }
  }

  /* Scrollbar */
  /* Credit to @devinrhode2 on GitHub: https://gist.github.com/devinrhode2/2573411 */
  ::-webkit-scrollbar {
    width: 8px;
    background-color: rgba(0, 0, 0, 0);
    -webkit-border-radius: 100px;
    border-radius: 100px;
  }

  ::-webkit-scrollbar:hover {
    background-color: rgba(255, 255, 255, 0.05);
    transition: background-color 0.5 ease;
  }

  ::-webkit-scrollbar-thumb:vertical {
    background: rgba(255, 255, 255, 0.3);
    -webkit-border-radius: 100px;
    border-radius: 100px;
  }
  ::-webkit-scrollbar-thumb:vertical:active {
    background: rgba(255, 255, 255, 0.2);
    -webkit-border-radius: 100px;
    border-radius: 100px;
  }
</style>
