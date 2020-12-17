<template>
  <div id="review">
    <div class="w-3/5 m-auto my-8 text-xl font-normal text-center rounded lg:text-left lg:w-1/2 lg:text-2xl xl:text-3xl">
      {{ title }}
    </div>
    <div class="w-3/5 mx-auto mt-8 text-center lg:w-1/2">
      <div class='card-container'>
        <div class='py-12 text-xl font-normal rounded bg-nord0 card lg:text-2xl xl:text-3xl'>
          <div>
            {{ $store.state.decks.currDeck.characters[0].char }}
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import { ReviewResult } from '../interfaces/ReviewResult'

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
        required: true
      }
    },
    data() {
      return {
        currReviewIndex: 0,
        results: Array<ReviewResult>(this.$store.state.currDeck.characters.length),
      }
    },
    methods: {
      sendResults(): void {
        this.$store.dispatch('decks/sendReviewResults', {
          deck: this.id,
          results: this.results,
        })
      },
    },
    created() {
      this.$store.dispatch('decks/getCardsToReview', this.id)
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

@media(min-width: 1024px) {
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

