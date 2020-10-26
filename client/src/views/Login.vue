<template>
  <div id='login'>
    <div class='w-4/5 mx-auto md:w-2/5'>
      <form @submit.prevent='' class='px-8 py-8 bg-gray-800 rounded'>
        <div class='my-4'>
          <label for='email' class='block mb-1'>Email</label>
          <input
            class='w-full px-3 py-1 text-gray-800 bg-gray-200 rounded'
            v-model='email' placeholder='alice@example.com' type='text'
          >
        </div>
        <div class='my-6'>
          <label for='password' class='block mb-1'>Password</label>
          <input
            class='w-full px-3 py-1 text-gray-800 bg-gray-200 rounded'
            v-model='password' placeholder='••••••••' type='password'
          >
        </div>
      </form>
    </div>
  </div>
</template>


<script lang="ts">
import { defineComponent } from 'vue'

export default defineComponent({
  name: 'Login',
  data(): object {
    return {
      email: '',
      password: ''
    }
  }
})
</script>


<style scoped>

</style>
