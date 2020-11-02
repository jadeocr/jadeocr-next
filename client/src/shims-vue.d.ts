declare module '*.vue' {
  // eslint-disable-next-line
  import type { DefineComponent } from 'vue'
  const component: DefineComponent
  export default component
}
