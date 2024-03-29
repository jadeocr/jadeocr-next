<template>
  <div id="home">
    <div class="w-full h-auto">
      <div class="lg:grid lg:grid-cols-2 lg:mt-16 xl:-mt-12 md:-mt-12">
        <img
          :src="require('@/assets/title-image.png')"
          alt="An image of the Chinese character 玉"
          class="lg:col-span-1"
        />
        <div
          class="flex flex-col justify-center -mt-8 text-3xl font-normal text-center md:text-5xl lg:col-span-1 lg:text-left md:-mt-32 lg:-mt-8"
        >
          <p class="leading-normal">Learn languages</p>
          <p class="leading-normal">without distractions.</p>
          <start-link
            linkType="getStartedButton"
            class="mt-6 font-light md:mt-8"
          />
        </div>
      </div>
      <div class="mt-10 md:mt-16 xl:-mt-12">
        <svg
          class="mx-auto bi bi-chevron-down animate-bounce"
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
    <div>
      <div id="about" class="py-4">
        <div class="mt-4 md:mt-12">
          <div
            class="px-8 text-lg font-normal text-center md:text-2xl lg:text-3xl opacity-87"
          >
            <p>Welcome to a better way to learn languages.</p>
          </div>
          <div
            class="px-3 mt-6 text-sm font-normal text-center md:text-lg lg:text-xl opacity-87"
          >
            <div>
              Learn languages with
              <a
                href="https://en.wikipedia.org/wiki/Spaced_repetition"
                target="_blank"
                class="font-semibold hoverlink"
                >spaced repetition</a
              >
              flashcards
            </div>
            <p>and practice writing with built-in handwriting recognition.</p>
          </div>
          <div
            class="flex items-center justify-center w-1/2 m-auto lg:mt-10 grid grid-rows-8 grid-cols-1 lg:grid-cols-2 text-md md:text-lg"
          >
            <div class="col-span-1 lg:pr-8">
              <Feature
                name="minimalist"
                :isFirstElement="true"
                title="Minimalist Design"
                body="
                  A modern, simple design with the Nord color palette makes using
                  jadeocr fun and intuitive.
                "
              />
              <Feature
                name="ocr"
                :isFirstElement="false"
                title="Handwriting OCR"
                body="
                  Computer vision delivers powerful optical character recognition 
                  to help you practice handwriting.
                "
              />
              <Feature
                name="cloud"
                :isFirstElement="false"
                title="Cloud Sync"
                body="
                  Jadeocr safely stores your decks in the cloud so you 
                  can review no matter where you are.
                "
              />
            </div>
            <div class="col-span-1 lg:pl-8">
              <Feature
                name="reactive"
                :isFirstElement="true"
                title="Reactive UI"
                body="
                  Vue.js powers jadeocr's snappy experience, making 
                  learning new languages a joy.
                "
              />
              <Feature
                name="spaced-repetition"
                :isFirstElement="false"
                title="SRS Cards"
                body="
                  Spaced repetition allows you to learn more efficiently 
                  and promotes long-term retention.
                "
              />
              <Feature
                name="open-source"
                :isFirstElement="false"
                title="Open Source"
                body="
                  In the open-source spirit, the
                  code is public on GitHub. Thus,  
                  jadeocr will remain free, forever.
                "
              />
            </div>
          </div>
        </div>
      </div>
      <div class="mt-24 footer">
        <hr class="opacity-25" />
        <div id="footer-content" class="py-8 text-xs font-normal lg:text-sm">
          <div class="text-center grid grid-cols-1 lg:grid-cols-2 opacity-87">
            <div class="w-1/2 m-auto text-center col-span-1">
              <p>Tanay Biradar, Jeremy Tow, Riley Kong</p>
              <p>
                This project is licensed under the
                <a
                  href="https://github.com/TanayB11/jadeocr/blob/master/LICENSE"
                  target="_blank"
                  class="font-semibold hoverlink"
                >
                  MIT License.
                </a>
              </p>
            </div>
            <!-- Make 2 more cols on small displays -->
            <div
              class="w-1/2 m-auto mt-4 font-semibold text-center col-span-1 lg:mt-0"
            >
              <div class="grid grid-cols-2 lg:grid-cols-1">
                <div class="col-span-1">
                  <div>
                    <a
                      href="https://github.com/jadeocr/jadeocr-next"
                      target="_blank"
                      class="hoverlink"
                      >GitHub</a
                    >
                    |
                    <a
                      href="https://docs.jadeocr.com/"
                      target="_blank"
                      class="hoverlink"
                      >API</a
                    >
                  </div>
                  <div>
                    <a
                      href="https://github.com/TanayB11/jadeocr#Donations"
                      target="_blank"
                      class="hoverlink"
                      >Donate</a
                    >
                  </div>
                </div>
                <div class="col-span-1">
                  <div>
                    <a href="/terms" target="_blank" class="hoverlink">Terms</a>
                  </div>
                  <div>
                    <a href="mailto: devs@jadeocr.com" class="hoverlink"
                      >Contact</a
                    >
                  </div>
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
  import StartLink from '@/components/StartLink.vue'
  import Feature from '@/components/Feature.vue'
  import { defineComponent } from 'vue'

  export default defineComponent({
    name: 'Home',
    components: {
      StartLink,
      Feature,
    },
  })
</script>

<style scoped>
  .slide {
    animation: 0.75s slidedown;
  }

  #scroll-cue {
    animation: pulse 3s 5s ease-in-out infinite;
  }

  @keyframes pulse {
    50% {
      transform: scale(0.8);
    }

    100% {
      transform: none;
    }
  }

  @keyframes slidedown {
    from {
      margin-top: -5vh;
      opacity: 0;
    }
    to {
      margin-top: 0vh;
      opacity: 0.87;
    }
  }
  /* To deal with weird landscape bug on phones */
  @media (max-width: 800px) and (orientation: landscape) {
    #intro {
      height: 200vh;
      margin-bottom: 8rem;
    }
  }
</style>
