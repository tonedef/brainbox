Import("env")

def after_upload(source, target, env):
    print("Delaying so port enumeration can finish before starting tests")
    import time
    time.sleep(5)
    print("Done!")

env.AddPostAction("upload", after_upload)