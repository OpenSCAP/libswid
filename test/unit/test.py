import swid


def _test_backend(backend_name):
    backend = swid.get_swidtagio(backend_name)
    # TODO: This causes segfaults.
    """
    try:
        backend.load("")
        assert 0
    except swid.XMLIOError as exc:
        assert str(exc).startswith("Could not load from '':")
        pass
    """

    data = swid.SWIDStruct()

    data.name = "ACME RoadRunner Management Suite"
    data.tagId = "com.acme.rms-ce-v4-1-5-0"
    data.xml_lang = "en-us"

    fname = "{}.xml".format(backend_name)

    entity = swid.SWIDEntity()
    entity.name = "The ACME corporation"
    entity.regid = "acme.com"
    entity.role = swid.SWID_ROLE_SOFTWARE_CREATOR | swid.SWID_ROLE_TAG_CREATOR

    data.entities.append(entity)

    data.type = swid.SWID_TYPE_CORPUS

    backend.save(fname, data)

    loaded_data = backend.load(fname)
    assert loaded_data.name == data.name
    assert loaded_data.tagId == data.tagId
    assert loaded_data.type == data.type
    assert loaded_data.xml_lang == data.xml_lang
    assert data.version == loaded_data.version
    assert loaded_data.version == ""
    assert loaded_data.versionScheme == ""

    loaded_data.applyDefaults()
    assert loaded_data.version == "0.0"
    assert loaded_data.versionScheme == "multipartnumeric"

    assert len(loaded_data.entities) == 1
    assert loaded_data.entities[0] == data.entities[0]

    entity2 = swid.SWIDEntity()
    entity2.name = "Red Hat"
    entity2.role = swid.SWID_ROLE_DISTRIBUTOR
    data.entities.push_back(entity2)

    data.version = "1.2.3"
    data.versionScheme = "semver"
    data.type = swid.SWID_TYPE_PATCH

    link = swid.SWIDLink()
    link.href = "swid.com.acme.rms-ce-v4-1-5-0"
    link.rel = "patches"
    data.links.push_back(link)

    backend.save(fname, data)

    loaded_data = backend.load(fname)

    assert loaded_data.links.size() == 1

    assert loaded_data.entities.size() == 2
    assert loaded_data.entities[0] == data.entities[0]
    assert loaded_data.entities[1] == data.entities[1]
    assert loaded_data.version == data.version
    assert loaded_data.versionScheme == data.versionScheme


def test_xerces():
    return _test_backend("xerces")


def test_tinyxml():
    return _test_backend("tinyxml")
