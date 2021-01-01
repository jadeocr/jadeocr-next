<template>
  <div id="class">
    <div class="grid grid-cols-1 md:grid-cols-4">
      <div class="col-span-1">
        <sidebar />
      </div>
      <div class="mt-12 overflow-y-auto md:mt-16 col-span-1 page-content">
        <div class="mx-6 md:mx-4 lg:mx-10 xl:mx-20">
          <div
            class="text-2xl font-normal text-center md:text-3xl md:text-left"
          >
            {{ currClass.name }}
          </div>
          <div
            class="mt-1 text-lg font-normal text-center md:text-xl text-nord5 md:text-left"
          >
            {{ currClass.description }}
          </div>
          <div class="grid grid-cols-1 md:grid-cols-2">
            <div class="mt-10 col-span-1"></div>
            <div
              class="mt-10 text-center md:pr-12 md:text-right xl:mx-48 xl:w-1/3 col-span-1"
            >
              <div class="text-xl font-normal md:text-2xl">
                Class Info
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import Sidebar from '../components/Sidebar.vue'
  import { ClassI } from '../interfaces/Class'

  export default defineComponent({
    name: 'Class',
    props: {
      classCode: {
        type: String,
        required: true,
      },
    },
    data() {
      return {
        currClass: {} as ClassI,
      }
    },
    components: {
      Sidebar,
    },
    mounted() { // find name of class
      // note: can store in order of class code and do a binary search for
      // O(log n) time instead of O(n)
      const allClasses = this.$store.state.classes.classes.concat(this.$store.state.classes.classesTeaching)
      for (let i = 0; i < allClasses.length; i++) {
        if (allClasses[i].classCode == this.classCode) {
          this.currClass = allClasses[i]
        }
      }
    }
  })
</script>

<style scoped></style>
