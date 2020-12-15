<template>
  <div id="review">

  </div> 
</template>

<script lang="ts">
  import { defineComponent } from 'vue'

  export default defineComponent({
    name: 'Review',
    props: {
      id: {
        type: String,
        required: true
      }
    },
    mounted() {
      this.$store.dispatch('decks/getCardsToReview', this.id)
    }
  })
</script>

<style scoped>
</style>
