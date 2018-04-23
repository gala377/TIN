    -----------------------------------------------------------------------
    -- Debug stuff---------------------------------------------------------
    local debug_level = {
        DISABLED = 0,
        LEVEL_1  = 1,
        LEVEL_2  = 2
    }
    
    local DEBUG = debug_level.LEVEL_2
    
    local default_settings = {
        debug_level  = DEBUG,
        enabled      = true, -- whether this dissector is enabled or not
        port         = 2620, -- default TCP port number for FPM
        max_msg_len  = 4096, -- max length of FPM message
    }
    
    
    local dprint = function() end
    local dprint2 = function() end
    local function resetDebugLevel()
        if default_settings.debug_level > debug_level.DISABLED then
            dprint = function(...)
                info(table.concat({"Lua: ", ...}," "))
            end
    
            if default_settings.debug_level > debug_level.LEVEL_1 then
                dprint2 = dprint
            end
        else
            dprint = function() end
            dprint2 = dprint
        end
    end
    -- call it now
    resetDebugLevel()
    
    
    local go_proto = Proto("GO", "GryphOn Header")

    --------------------------------------------------------------------
    --Our protocol header

    local hdr_fields = {
        package_id = ProtoField.uint32("go.package", "Package", base.DEC),
        type_id = ProtoField.uint32("go.type", "Type", base.DEC),
        msg_len = ProtoField.uint32("go.length", "Length", base.DEC)
    }

    go_proto.fields = hdr_fields

    dprint2("go_proto fields registered")


    local tvbs = {}
    -- this function is called by wireshark at program start
    function go_proto.init()
        tvbs = {}
    end

   
   
   
    local GO_MSG_HDR_LEN = 12
    -- forward declarations
    local dissectGO, checkGoLength
    -- plain data Dissector
    local data = Dissector.get("data")
    -----------------------------------------------------
    --DISSECTOR-------------------------------------------
    function go_proto.dissector(tvbuf, pkt_info, root)
        dprint2("go_proto.dissector called")
        tvbs = {}
        --- i dont need to have my own tvbs do I?
        local pkt_len = tvbuf:len()
        local bytes_consumed = 0

        while bytes_consumed < pkt_len do
            
            result = dissectGO(tvbuf, pkt_info, root, bytes_consumed)
            if result > 0 then
                -- successfully dissected the message
                bytes_consumed = bytes_consumed + result
            elseif result == 0 then
                -- thats an error
                return 0 
            else
                pkt_info.desegment_offset = bytes_consumed
                pkt_info.desegment_len = -result
                return pkt_len
            end            
        end
        -- wireshark needs it
        return bytes_consumed
    end

    dissectGO = function(tvbuf, pkt_info, root, offset)
        dprint2("DissectGo called")
        local length_val, length_tvbr = checkGoLength(tvbuf, offset)

        if length_val <= 0 then
            return length_val
        end

        --- set pkt_info cols names
        pkt_info.cols.protocol:set("GO")
        if string.find(tostring(pkt_info.cols.info), "^GO") == nil then
            pkt_info.cols.info:set("GO")
        end
        -- add protocol to the display tree
        local tree = root:add(go_proto, tvbuf:range(offset, length_val))

        dprint2("dissecting package id")
        -- dissect package id 
        local package_id_vtbuf = tvbuf:range(offset, 4)
        -- should i maybe use uint instead?
        local package_id_val = package_id_vtbuf:uint()
        -- should we really add tvbuf thing? 
        tree:add(hdr_fields.package_id, package_id_vtbuf)
        dprint2("dissecting package type")
        -- dissect type id
        local package_type_tvbuf = tvbuf(offset+4, 4)
        local package_type_val = package_type_tvbuf:uint()
        tree:add(hdr_fields.type_id, package_type_tvbuf)

        dprint2("dissectiong length")
        --dissect the lenght
        tree:add(hdr_fields.msg_len, length_tvbr)

        dprint2("invoking data dissector")
        -- is the GO: <-- ':' sign correct? 
        if string.find(tostring(pkt_info.cols.info), "^GO:") == nil then
            pkt_info.cols.info:append(": Uknown")
        else
            pkt_info.cols.info:append(", Uknown")
        end
        -- is tvbs good here? i don't know
        tvbs[#tvbs+1] = tvbuf(offse + GO_MSG_HDR_LEN, length_val - GO_MSG_HDR_LEN):tvb()
        data:call(tvbs[#tvbs], pkt_info, root)

        return length_val
    end

    checkGoLength = function(tvbuf, offset)
        local msg_len = tvbuf:len() - offset
        if msglen ~= tvbuf:reported_length_remaining(offset) then
            dprint2("aptured packet was shorter than original, can't reassemble")
            return 0
        end 

        if msg_len < GO_MSG_HDR_LEN then
            dprint2("Need more bytes to figure out the GO length field")
            return -DESEGMENT_ONE_MORE_SEGMENT
        end

        local length_tvbr = tvbuf:range(offset+8, 4)
        dprint2("Big endian convert")
        -- do I really need to do theese?
        -- should I replace adll uint32 to uint? 
        local length_val = length_tvbr:uint()
        
        -- I could just delete it to be honest
        if length_val > default_settings.max_msg_len then
            --- too many bytes invalid msg
            dprint("GO msg len is to long: ", length_val)
            return 0
        end

        if msg_len < length_val then
            dprint2("Need more bytes to desegment full GO")
            return -(length_val - msg_len)
        end

        return length_val, length_tvbr
    end

    -- setting the specific port
    local function enableDissector()
        DissectorTable.get("tcp.port"):add(default_settings.port, go_proto)
    end
    enableDissector()

    local function disableDissector()
        DissectorTable.get("tcp.port"):remove(default_settings.port, go_proto)
    end

    --- preferences stuff

    local debug_pref_enum = {
        { 1,  "Disabled", debug_level.DISABLED },
        { 2,  "Level 1",  debug_level.LEVEL_1  },
        { 3,  "Level 2",  debug_level.LEVEL_2  },
    }

    -- register our preferences
    fpm_proto.prefs.enabled     = Pref.bool("Dissector enabled", default_settings.enabled,
                                            "Whether the FPM dissector is enabled or not")
    fpm_proto.prefs.debug       = Pref.enum("Debug", default_settings.debug_level,
                                            "The debug printing level", debug_pref_enum)

    -- the function for handling preferences being changed
    function fpm_proto.prefs_changed()
        dprint2("prefs_changed called")
        default_settings.debug_level = fpm_proto.prefs.debug
        resetDebugLevel()
    
        if default_settings.enabled ~= fpm_proto.prefs.enabled then
            default_settings.enabled = fpm_proto.prefs.enabled
            if default_settings.enabled then
                enableDissector()
            else
                disableDissector()
            end
            -- have to reload the capture file for this type of change
            reload()
        end
    
    end
    
    dprint2("pcapfile Prefs registered")