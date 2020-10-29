<template>
  <div id='auth-form'>
    <div class='w-4/5 px-8 py-8 mx-auto mt-10 bg-gray-800 rounded md:w-1/2 xl:w-1/4 lg:w-1/3'>
      <!-- TODO: Add submit action -->
      <form @submit.prevent=''>
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
        <!-- Signup page only -->
        <div v-if='formType == "signup"' class='my-6'>
          <label for='password' class='block mb-1'>Confirm Password</label>
          <input
            class='w-full px-3 py-1 text-gray-800 bg-gray-200 rounded'
            v-model='confirmPassword' placeholder='••••••••' type='password'
          >
        </div>
      </form>
      <!-- Login/signup buttons -->
      <div v-if='formType == "login"'>
        <div class='flex items-center justify-between mt-8'>
          <button class='px-4 py-2 bg-gray-600 rounded'>
            Log In
          </button> 
          <router-link :to='{ name: "Signup" }' class='px-4 py-2'>
            Sign Up
          </router-link>
        </div>
      </div>
      <div v-else>
        <div class='flex items-center justify-between mt-8'>
          <button class='px-4 py-2 bg-gray-600 rounded'>
            Sign Up
          </button> 
          <router-link :to='{ name: "Login" }' class='px-4 py-2'>
            Login
          </router-link>
        </div>
      </div>
    </div>
  </div>
</template>


<script lang="ts">
import { defineComponent } from 'vue'

export default defineComponent({
  name: 'Login',
  props: {
    formType: {
      type: String,
      required: true,
      validator: (value: string): boolean => {
        return ['login', 'signup'].includes(value)
      }
    }
  },
  data(): object {
    return {
      email: '',
      password: '',
      confirmPassword: ''
    }
  }
})
</script>


<style scoped>

</style>
