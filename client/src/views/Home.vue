<template>
  <div id='home'>
    <div id='intro'>
      <div class='m-auto text-center grid grid-cols-1 lg:grid-cols-2'>
        <div class='col-span-1'>
          <img src='../assets/title-image.png' alt='An image of the Chinese character 玉'>
        </div>
        <div class='-mt-8 md:-mt-32 col-span-1 lg:mt-0 lg:my-auto'>
          <div class='text-3xl text-center lg:text-5xl md:text-4xl lg:text-left'>
            <p>Learn languages</p>
            <p>without distractions.</p>
          </div>
          <div>
            <start-link linkType='getStartedButton' class='mt-6'/>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>


<script lang='ts'>
import { defineComponent } from 'vue'
import StartLink from '../components/StartLink.vue'

export default defineComponent({
  name: 'Home',
  components: {
    StartLink
  }
})
</script>
