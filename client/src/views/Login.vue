<template>
  <div id='login'>
    <auth-form formType='login'/> 
  </div>
</template>


<script lang="ts">
import { defineComponent } from 'vue'
import AuthForm from '../components/AuthForm.vue'

export default defineComponent({
  name: 'Login',
  components: {
    AuthForm
  },
  data(): object {
    return {
      email: '',
      password: ''
    }
  }
})
</script>
