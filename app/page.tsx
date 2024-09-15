import { OAuthLogin } from "@/components/OAuthLogin"
import InitUser from "@/lib/store/initUser";
import { createClient } from "@/lib/supabase/server"

export default async function App() {
  const supabase = createClient()
  //  const { data } = await supabase.auth.getSession();
  const { data } = await supabase.auth.getUser();

  //  console.log(data.session?.user)
  console.log('usr: ', data.user)

  return (
    <>
      <div className="max-w-3xl mx-auto md: py-10 h-screen ">
        <div className="h-full border rounded-md flex flex-col">

          <OAuthLogin usr={data.user} />

          <div className="flex h-full w-full">

            <div className="w-2/6 border-r">
            </div>

            <div className="w-2/3">
            </div>

          </div>
        </div>

      </div>

      <InitUser user={data.user!} />
    </>
  )
}
