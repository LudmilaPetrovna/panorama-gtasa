local CameraBuildParallelViewMatrix_addr=0x7EF1F0;
local CameraBuildParallelViewMatrix_before_optimize_addr=0x7EF32F;
local RwCameraSync_at_end_addr=0x7EE727;
local RwCameraSyncParent=0x7EE709
local CameraBuildPerspClipPlanes_addr=0x7EE740
local SceneCam=readInteger(0xC17038+4)

debug_setBreakpoint(CameraBuildParallelViewMatrix_addr)
debug_setBreakpoint(CameraBuildPerspClipPlanes_addr)

getLuaEngine().cbShowOnPrint.Checked=false
getLuaEngine().show()

local addressList = getAddressList()
local hits={}

-- add switch
local anr=addressList.createMemoryRecord()
anr.Description="parallel mode"
anr.Address=SceneCam+0x14
anr.Type=vtByte
anr.ShowAsHex=true

local nr=addressList.createMemoryRecord()
nr.Description=msg
nr.Address=""..string.format("0x%X",SceneCam)..""
nr.Type=vtByteArray
nr.ShowAsHex=true
nr.Aob.Size=388

nr=addressList.createMemoryRecord()
nr.Description="Matrix of..."
nr.Address=SceneCam+0x20
nr.Type=vtByteArray
nr.ShowAsHex=true
nr.Aob.Size=64

debugger_onBreakpoint = function()
    if EIP == CameraBuildParallelViewMatrix_addr then
        local parent=readInteger(ESP)
        local rwcam=readInteger(ESP+0x4)
        local rwcam_type=readInteger(rwcam+0x14)

--    if parent ~= RwCameraSyncParent then -- we expect call from RwCameraSync
--    return 1
--    end

        if rwcam ~= SceneCam then -- we expect main scene camera
            return 1
        end

        local msg=string.format("rwcam at: 0x%X, type: %d", rwcam, rwcam_type)

        if hits[msg] == nil then
            hits[msg] = 1
        else
            hits[msg] = hits[msg] + 1

            -- kick LTM
            local frame=readInteger(rwcam+4)
            local ltm=readInteger(frame+0x50)
            for qq=0, 15 do
    --               writeFloat(ltm+qq*4,math.random(0.0, 10.0))
                     writeInteger(ltm+qq*4,qq+128)
            end
        end

        GetLuaEngine().MenuItem5.doClick() --clean window
        print(dump(hits))
        return 1

    end

    if EIP==CameraBuildPerspClipPlanes_addr then
        local rwcam=SceneCam
        local matrix_addr=rwcam+0x20
        GetLuaEngine().MenuItem5.doClick() --clean window
        for qq=0, 15 do
          local par=matrix_addr+qq*4
--        writeFloat(par,qq)
          writeInteger(par,qq)
          print(string.format("Matrix patched at 0x%X by %d",par,qq))
        end
        return 1
    end

end

function dump(o)
   if type(o) == 'table' then
      local s = '{ '
      for k,v in pairs(o) do
         if type(k) ~= 'number' then k = '"'..k..'"' end
         s = s .. '['..k..'] = ' .. dump(v) .. ',\n'
      end
      return s .. '} '
   else
      return tostring(o)
   end
end
