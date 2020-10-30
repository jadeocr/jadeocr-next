<template>
  <div id="home">
    <div id="intro">
      <div
        class="mx-auto text-center grid grid-cols-1 lg:grid-cols-2 md:-mt-16 lg:mt-16 xl:-mt-8"
      >
        <div class="col-span-1">
          <img
            src="../assets/title-image.png"
            alt="An image of the Chinese character 玉"
          />
        </div>
        <div
          class="flex flex-col justify-center -mt-8 text-center md:-mt-32 col-span-1 lg:mt-0 lg:text-left xl:ml-16"
        >
          <div class="text-3xl lg:text-5xl md:text-4xl">
            <p>Learn languages</p>
            <p>without distractions.</p>
          </div>
          <div>
            <start-link
              linkType="getStartedButton"
              class="mt-6 md:mt-10 lg:mt-6"
            />
          </div>
        </div>
      </div>
      <div class="mt-8 md:mt-16 grid grid-rows-1 lg:-mt-10 xl:-mt-20">
        <div class="m-auto row-span-1">
          <svg
            id="scroll-cue"
            class="bi bi-chevron-down"
            width="3em"
            height="3em"
            viewBox="0 0 16 16"
            fill="currentColor"
            xmlns="http://www.w3.org/2000/svg"
          >
            <path
              fill-rule="evenodd"
              d="M1.646 4.646a.5.5 0 01.708 0L8 10.293l5.646-5.647a.5.5 0 01.708.708l-6 6a.5.5 0 01-.708 0l-6-6a.5.5 0 010-.708z"
              clip-rule="evenodd"
            />
          </svg>
        </div>
      </div>
    </div>
    <div id="about">
      <!-- TODO: Add about content here -->
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import StartLink from '../components/StartLink.vue'

  export default defineComponent({
    name: 'Home',
    components: {
      StartLink,
    },
  })
</script>

<style scoped>
  /* TODO: Add scroll cue animation */
</style>
