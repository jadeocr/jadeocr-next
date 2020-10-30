<template>
  <div id="startlink">
    <div
      v-if="linkType == 'navigation'"
      id="navigation"
      class="w-1/2 m-auto text-center mt-7"
    >
      <nav class="text-2xl md:text-3xl xl:text-4xl">
        <router-link :to="{ name: navRoute }">jadeocr</router-link>
      </nav>
    </div>
    <div v-if="linkType == 'getStartedButton'" id="getStartedButton">
      <!-- TODO: Change button color back to green after designing page -->
      <router-link
        :to="{ name: navRoute }"
        class="px-3 py-1 text-2xl bg-nord1 rounded-md md:text-3xl xl:text-4xl"
      >
        Get Started 登录
      </router-link>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'

  export default defineComponent({
    name: 'StartLink',
    props: {
      linkType: {
        type: String,
        required: true,
        validator: (value: string): boolean => {
          return ['navigation', 'getStartedButton'].includes(value)
        },
      },
    },
    computed: {
      navRoute(): string {
        const tempCondition = false // TODO: Change to check for this.$store.state.signedIn
        if (this.linkType == 'getStartedButton') {
          return 'Learn'
        } else if (tempCondition) {
          return 'Learn'
        } else {
          return 'Home'
        }
      },
    },
  })
</script>

<style scoped></style>
