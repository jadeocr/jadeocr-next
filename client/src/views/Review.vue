<template>
  <div id="review">
    <div
      class="w-3/5 m-auto mt-8 mb-2 text-xl font-normal text-center rounded md:mt-16 lg:text-left lg:w-1/2 lg:text-2xl xl:text-3xl md:text-3xl"
    >
      {{ title }}
    </div>
    <div class="w-3/5 mx-auto mt-8 text-center lg:w-1/2">
      <div class="card-container">
        <div
          class="px-6 py-12 text-xl font-normal rounded bg-nord0 card lg:text-2xl xl:text-3xl md:text-3xl"
        >
          <div>
            <p
              v-for="(n, side) in visibleCardData"
              :key="side.key"
              class="font-light chinese"
            >
              {{ visibleCardData[side] }}
            </p>
          </div>
        </div>
      </div>
      <div class="mt-10">
        <div
          class="flex items-center justify-between m-auto mt-4 md:w-2/3 opacity-87"
        >
          <div
            class="px-4 py-3 rounded-md bg-nord7"
            @click="cardCheck('correct')"
          >
            <svg
              xmlns="http://www.w3.org/2000/svg"
              width="1.1em"
              fill="currentColor"
              class="bi bi-check2"
              viewBox="0 0 16 16"
            >
              <path
                fill-rule="evenodd"
                d="M13.854 3.646a.5.5 0 0 1 0 .708l-7 7a.5.5 0 0 1-.708 0l-3.5-3.5a.5.5 0 1 1 .708-.708L6.5 10.293l6.646-6.647a.5.5 0 0 1 .708 0z"
              />
            </svg>
          </div>
          <div
            class="px-4 py-2 mx-auto font-normal text-md rounded-md md:text-lg unselect bg-nord9"
            @click="flipCard()"
          >
            {{ type == 'flashcards' ? 'Flip Card' : 'Check Writing' }}
          </div>
          <div
            class="px-4 py-3 btn bg-nord12 rounded-md"
            @click="cardCheck('incorrect')"
          >
            <svg
              xmlns="http://www.w3.org/2000/svg"
              width="1.1em"
              fill="currentColor"
              class="bi bi-x"
              viewBox="0 0 16 16"
            >
              <path
                fill-rule="evenodd"
                d="M4.646 4.646a.5.5 0 0 1 .708 0L8 7.293l2.646-2.647a.5.5 0 0 1 .708.708L8.707 8l2.647 2.646a.5.5 0 0 1-.708.708L8 8.707l-2.646 2.647a.5.5 0 0 1-.708-.708L7.293 8 4.646 5.354a.5.5 0 0 1 0-.708z"
              />
            </svg>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import { ReviewResult } from '../interfaces/ReviewResult'
  import { Character } from '../interfaces/Character'
  import axios from 'axios'
  const apiBaseURL = process.env.VUE_APP_API_BASEURL

  export default defineComponent({
    name: 'Review',
    props: {
      type: {
        type: String,
        required: true,
        validator: (value: string): boolean => {
          return ['flashcards', 'ocr', 'quiz'].includes(value)
        },
      },
      id: {
        type: String,
        required: true,
      },
      title: {
        type: String,
        required: true,
      },
    },
    data() {
      return {
        cards: Array<Character>(),
        // eslint-disable-next-line
        visibleCardData: [] as any,
        currReviewIndex: 0,
        results: Array<ReviewResult>(),
      }
    },
    methods: {
      cardCheck(correctness: string): void {
        this.results.push({
          id: this.cards[this.currReviewIndex].id,
          quality: correctness == 'correct' ? 5 : 0,
        } as ReviewResult)
        if (this.currReviewIndex + 1 == this.cards.length) {
          this.$store.dispatch('decks/sendReviewResults', this.results)
        } else {
          this.currReviewIndex++
          this.visibleCardData = [
            this.cards[this.currReviewIndex].pinyin,
            this.cards[this.currReviewIndex].definition,
          ]
        }
      },
      resetVisibleCard(): void {
        this.visibleCardData = [
          this.cards[this.currReviewIndex].pinyin,
          this.cards[this.currReviewIndex].definition,
        ]
      },
      flipCard(): void {
        if (this.visibleCardData.length == 3) {
          this.resetVisibleCard()
        } else {
          this.visibleCardData.push(this.cards[this.currReviewIndex].char)
        }
      },
      sendResults(): void {
        this.$store.dispatch('decks/sendReviewResults', {
          deck: this.id,
          results: this.results,
        })
      },
      getCardsToReview(): void {
        axios({
          method: 'post',
          url: `${apiBaseURL}/deck/srs`,
          withCredentials: true,
          data: {
            deckId: this.id,
          },
        })
          .then((res) => {
            this.cards = res.data
            this.resetVisibleCard()
          })
          .catch((err) => {
            console.log(err.response.data)
          })
      },
    },
    mounted() {
      this.getCardsToReview()
    },
  })
</script>

<style scoped>
  .card {
    z-index: 2;
    width: 100%;
    height: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  @media (max-width: 640px) {
    .card {
      height: 50vh;
    }
  }
  @media (min-width: 768px) {
    .card {
      height: 50vh;
    }
  }
  @media (min-width: 1024px) {
    .card-container {
      display: flex;
      justify-content: space-between;
    }
    .card {
      height: 40vh;
    }
    #draw-wrapper {
      width: 80vh;
      height: 40vh;
    }
    .bi {
      width: 1.25em;
      height: 1.25em;
    }
  }
</style>

