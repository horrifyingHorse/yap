"use client"

import { Button } from "@/components/ui/button";
import { createClient } from "@/lib/supabase/client";
import { User } from "@supabase/supabase-js";
import { useRouter } from "next/navigation";

export function OAuthLogin({ usr }: { usr: User | null }) {
  const router = useRouter()

  const handleSignInWithGoogle = () => {
    const supabase = createClient()
    supabase.auth.signInWithOAuth({
      provider: "google",
      options: {
        redirectTo: location.origin + "/auth/callback"
      }
    })
  }

  const handleLogout = async () => {
    const supabase = createClient()
    await supabase.auth.signOut()
    router.refresh()
  }

  return (
    <div className="w-full h-14 border-b p-2 flex items-center justify-between" >
      <div className="flex items-center gap-1">
        <div className="h-3 w-3 rounded-full bg-emerald-500" />
        Online
      </div>
      {usr
        ? <Button onClick={handleLogout}> Logout </Button>
        : <Button onClick={handleSignInWithGoogle}> Login </Button>
      }
    </div >
  )
}
